//          Copyright Stefan Strasser 2009 - 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_TRANSACT_LOG_HEADER_HPP
#define BOOST_TRANSACT_LOG_HEADER_HPP

#include <cstring>
#include <string>
#include <algorithm>
#include <iterator>
#include <set>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/optional.hpp>
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/thread/locks.hpp>
#include <boost/utility/in_place_factory.hpp>
#include <boost/transact/detail/sectorizing_file.hpp>
#include <boost/transact/detail/aligning_file.hpp>
#include <boost/transact/detail/buffering_file.hpp>
#include <boost/transact/detail/syncing_file.hpp>
#include <boost/transact/detail/filebuf_file.hpp>
#include <boost/transact/detail/embedded_vector.hpp>


namespace boost{
namespace transact{


namespace detail{

template<class OutputIterator>
static void get_existing_log_ids(OutputIterator out,std::string const &name){
	namespace fs=boost::filesystem;
	try{
		fs::path dir=fs::path(name).remove_filename();
		std::string filename=fs::path(name).filename();
		fs::directory_iterator it;
		if(dir.empty()) it=fs::directory_iterator(".");
		else it=fs::directory_iterator(dir);
		for(;it != fs::directory_iterator();++it){
			std::string foundfilename=it->path().filename();
			if(foundfilename.substr(0,filename.length()+1) != filename+".") continue;
			std::string nrstring=foundfilename.substr(filename.length()+1);
			unsigned int nr;
			try{ nr=lexical_cast<unsigned int>(nrstring); } catch(...){ continue; }
			*out++=nr;
		}
	}catch(...){
		throw io_failure();
	}
}
static unsigned int get_next_log_id(std::string const &name){
	std::vector<unsigned int> ids;
	get_existing_log_ids(std::back_inserter(ids),name);
	return ids.empty() ? 1 : *std::max_element(ids.begin(),ids.end())+1;
}

static std::string get_log_filename(std::string const &name,unsigned int l){
	return name+'.'+lexical_cast<std::string>(l);
}

}


template<bool Sync>
class olog_files{
public:
	olog_files(std::string const &name,unsigned int max_log_size)
		: log_id(detail::get_next_log_id(name))
		, rolling_(false)
		, max_log_size(max_log_size)
		, header_size(0)
		, name(name)
		, file(new file_type(detail::get_log_filename(name,log_id))){}

	//Archive interface
	template<class T>
	olog_files &operator<<(T const &t){
		this->file->save(t);
		return *this;
	}
	template<class T>
	olog_files &operator&(T const &t){ return this->operator<<(t); }
	template<class Size>
	void save_binary(void const *data,Size size){
		this->file->save_binary(data,size);
	}
	typedef mpl::true_ is_saving;
	typedef mpl::false_ is_loading;


	bool overflow() const{
		return (!this->rolling())
			&& (this->file->position() > this->max_log_size + this->header_size);
	}

	
	bool rolling() const{ return this->rolling_; }
	template<class Header>
	void begin_roll(Header header){
		BOOST_ASSERT(!this->rolling());
		this->rolling_=true;
		try{
			//first construct the new log and then replace it. if construction
			//of new log file fails, there must still be a valid file in this->file
			//for other threads:
			this->file.reset(new file_type(detail::get_log_filename(this->name,this->log_id+1)));
			++this->log_id;
			header();
			this->header_size=this->file->position();
		}catch(...){
			this->rolling_=false;
			throw;
		}
	}

	void commit_roll(){
		BOOST_ASSERT(this->rolling_);
		this->flush();
		this->sync(); //TODO optimization: outside of lock
		//TODO optimization: don't remove, but invalidate and rename to the next log. increases
		//performance of writing ahead if the file is already fully "allocated".

		try{
			filesystem::remove(detail::get_log_filename(this->name,this->log_id-1));
		}catch(...){
			throw io_failure();
		}
		this->rolling_=false;
	}

	void flush(){
		this->file->flush();
	}
	void sync(){
		this->file->sync();
	}
private:
	typedef typename mpl::if_c<
		Sync,
		detail::sectorizing_seq_ofile<
			detail::aligning_seq_ofile<
				detail::buffering_seq_ofile<
					detail::syncing_seq_ofile,
					8192
				>
			>
		>,
		detail::buffering_seq_ofile<
			detail::filebuf_seq_ofile,
			8192
		>
	>::type file_type;

	unsigned int log_id;
	bool rolling_;
	typename file_type::size_type max_log_size;
	typename file_type::size_type header_size;
	std::string const name;
	scoped_ptr<file_type> file; //no optional. see begin_roll()
};

template<bool Sync>
class otransaction_log_files : public olog_files<Sync>{
private:
	struct detail{
		typedef unsigned int transaction;
	};
public:
	typedef typename detail::transaction transaction;
	otransaction_log_files(std::string const &name,std::size_t max_log_size)
		: olog_files<Sync>(name,max_log_size)
		, next_tx(1)
		, open_transactions(0)
		, rolled_transactions(0)
		, roll_cutoff(0){}

	template<class Header>
	typename detail::transaction begin_transaction(Header header){
		transaction tx=this->next_tx++;
		if(this->overflow()){
			this->begin_roll(header);
			this->roll_cutoff=tx;
			BOOST_ASSERT(this->rolled_transactions==0);
			this->rolled_transactions=this->open_transactions;
			this->open_transactions=0;
		}
		BOOST_ASSERT(!this->rolling() || tx >= this->roll_cutoff);
		++this->open_transactions;
		return tx;
	}
	void end_transaction(transaction tx){
		if(this->rolling() && tx < this->roll_cutoff){
			BOOST_ASSERT(this->rolled_transactions > 0);
			--this->rolled_transactions;
			if(this->rolled_transactions==0){
				//all transactions that were begun in the old, rolled, log have ended.
				this->commit_roll();
			}
		}else{
			BOOST_ASSERT(this->open_transactions > 0);
			--this->open_transactions;
		}
	}
private:
	transaction next_tx;
	std::size_t open_transactions;
	std::size_t rolled_transactions;
	transaction roll_cutoff;
};

template<bool Sync>
class ilog_files{
public:
	explicit ilog_files(std::string const &name) : name(name){
		detail::get_existing_log_ids(std::inserter(this->log_ids,this->log_ids.begin()),name);
		this->current=this->log_ids.begin();
	
		//although the current implementation of log rolling only produces a maximum of
		//2 logs at a time, an aborted recovery may have created another log. work through all of them.
	
		for(log_ids_type::const_iterator next=this->log_ids.begin();next!=this->log_ids.end();){
			log_ids_type::const_iterator it=next++;
			if(next != this->log_ids.end() && (*it != *next-1)) throw io_failure();
		}
	
		if(this->current != this->log_ids.end()){
			this->file=in_place(detail::get_log_filename(this->name,*this->current));
		}
	}
	//Archive interface
	template<class T>
	ilog_files &operator>>(T const &t){
		this->file->load(t);
		return *this;
	}
	template<class T>
	ilog_files &operator&(T const &t){ return this->operator>>(t); }
	template<class Size>
	void load_binary(void *data,Size size){
		this->file->load_binary(data,size);
	}
	typedef mpl::true_ is_saving;
	typedef mpl::false_ is_loading;
	void remove(){
		try{
			for(log_ids_type::const_iterator it=this->log_ids.begin();it != this->log_ids.end();++it){
				filesystem::remove(detail::get_log_filename(this->name,*it));
				//FIXME sync directory entry. the delete of an older log
				//must have reached disk before a newer log is deleted.
			}
		}catch(...){
			throw io_failure();
		}
	}
private:
	typedef std::set<unsigned int> log_ids_type;
	log_ids_type log_ids;
	log_ids_type::const_iterator current;
	std::string const name;
	typedef typename mpl::if_c<
		Sync,
		detail::sectorizing_seq_ifile<detail::filebuf_seq_ifile>,
		detail::filebuf_seq_ifile
	>::type file_type;
	optional<file_type> file;
};

namespace detail{

template<class Entry,class State>
struct make_entry_pair{
	typedef mpl::pair<
		Entry,
		typename mpl::size<State>::type
	> type;
};

template<class Vector>
struct invert_vector{
	typedef typename mpl::fold<
		Vector,
		mpl::map0<>,
		mpl::insert<
			mpl::_1,
			make_entry_pair<
				mpl::_2,
				mpl::_1
			>
		>
	>::type type;
};

}

template<class Entries,bool Sync>
class olog{
private:
	typedef typename detail::invert_vector<Entries>::type ids;
public:
	typedef unsigned char id_type;
	explicit olog(olog_files<Sync> &files)
		: files(files)
		, id_offset(0){}

	template<class T>
	id_type id() const{
		return mpl::at<ids,T>::type::value + this->id_offset;
	}

	template<class T>
	olog &operator<<(T const &t){
		id_type const tid=this->id<T>();
		static std::size_t const size=sizeof(tid) + sizeof(t);
		unsigned char data[size];
		std::memcpy(data,&tid,sizeof(tid));
		std::memcpy(data+sizeof(tid),&t,sizeof(t));
		this->files.save_binary(data,mpl::size_t<size>());
		return *this;
	}
	struct archive{
		explicit archive(olog &log) : log(log){}
		template<class T>
		archive &operator<<(T const &t){
			this->log.files << t;
			return *this;
		}
		template<class T>
		archive &operator&(T const &t){ return this->operator<<(t); }
		template<class Size>
		void save_binary(void const *data,Size size){
			this->log.files.save_binary(data,size);
		}
		typedef mpl::true_ is_saving;
		typedef mpl::false_ is_loading;
	private:
		olog &log;
	};
private:
	olog_files<Sync> &files;
	id_type id_offset;
};

template<class Log,class Lockable,std::size_t Size>
class olog_buffer{
public:
	typedef typename Log::id_type id_type;
	olog_buffer(Log &log,Lockable &lockable)
		: log(log), lockable(lockable){}
	template<class T>
	olog &operator<<(T const &t){
		static std::size_t const size=sizeof(id_type) + sizeof(T);
		BOOST_STATIC_ASSERT(size <= Size);
		id_type const id=this->log.template id<T>();	

		if(this->buffer.size() + size > this->buffer.max_size()) this->flush();
		this->buffer.push_back(reinterpret_cast<char const *>(&id),mpl::size_t<sizeof(id_type)>());
		this->buffer.push_back(reinterpret_cast<char const *>(&t),mpl::size_t<sizeof(T)>());
		return *this;
	}
	struct archive{
		explicit archive(olog_buffer &log) : log(log){}
		template<class T>
		archive &operator<<(T const &t){
			this->save_binary(&t,mpl::size_t<sizeof(T)>());
			return *this;
		}
		template<class T>
		archive &operator&(T const &t){ return this->operator<<(t); }
		template<class S>
		void save_binary(void const *data,S size){
			if(this->log.buffer.size() + size > this->log.buffer.max_size()){
				if(size > Size){
					this->log.flush_and_write(data,size);
					return;
				}else{
					this->log.flush();
				}
			}
			this->log.buffer.push_back(static_cast<char const *>(data),size);
		}
		typedef mpl::true_ is_saving;
		typedef mpl::false_ is_loading;
	private:
		olog_buffer &log;
	};
private:
	friend struct archive;

	void flush(){
		if(!this->buffer.empty()){
			typename Log::archive ar(this->log);
			{
				lock_guard<Lockable> l(this->lockable);
				ar.save_binary(&this->buffer[0],this->buffer.size());
			}
			this->buffer.clear();
		}
	}
	template<class S>
	void flush_and_write(void const *data,S size){
		typename Log::archive ar(this->log);
		lock_guard<Lockable> l(this->lockable);
		if(!this->buffer.empty()){
			ar.save_binary(&this->buffer[0],this->buffer.size());
			this->buffer.clear();
		}
		ar.save_binary(data,size);
	}


	Log &log;
	Lockable &lockable;
	detail::embedded_vector<char,Size,false> buffer;
};


}
}

#endif

