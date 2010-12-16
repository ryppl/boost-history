//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2010.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/frame for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_FRAME_FRAME_HPP
#define BOOST_FRAME_FRAME_HPP

//!\file
//! Describes a frame class that allows to push/pop a variable number of bytes 
//! efficiently.

namespace boost {
namespace frames {

    class frame {
    private:
        typedef std::size_t frame_size_t;
        frame_size_t capacity_;
        frame_size_t front_;
        frame_size_t back_;
        char* data_;
    protected:
        //!Constructor.
        //! <b>Throws<\b>: bad_alloc when no more memory available.
        //!
        frame(
                std::size_t front
                , std::size_t back
                , std::size_t payload
                , char*       data
                )
            : capacity_(front+back+payload)
            , front_(front)
            , back_(payload+front)
            , data_(data)

        {
        }

        //!Destructor.
        //! <b>Throws<\b>: nothing.
        //!
        ~frame() 
        {
        }
        char* data() 
        {
            return data_;
//            return reinterpret_cast<char*>(this)+sizeof(frame);
        }
    public:
        //! creates a frame having a capacity of front+back bytes.
        //! <b>Throws<\b>: bad_alloc whne no more memory available.
        template <typename Allocator>
        static frame* 
        make(std::size_t front, std::size_t back) 
        {
            void *ptr = Allocator::allocate(sizeof(frame)+front+back);
            return new(ptr) frame(front, back, 0,
                    reinterpret_cast<char*>(ptr)+sizeof(frame))
        }
        //! creates a frame having a capacity of front+back bytes.
        //! <b>Throws<\b>: bad_alloc whne no more memory available.
        template <typename Allocator>
        static frame* 
        make(std::size_t front, std::size_t back, std::size_t payload, void* data) 
        {
            void *ptr = Allocator::allocate(sizeof(frame));
            return new(ptr) frame(front, back, payload, data)
        }
        

        //! <b>Effects<\b>: destroy the  frm \a frm.
        //! 
        //! <b>Requires<\b>: \a frm has been created with frame::make.
        template <typename Allocator>
        static void 
        destroy(frame* frm)
        {
            frm.~frame();
            Allocator::deallocate(frm);
        }
        

        //! <b>Returns<\b> the number of bytes free from the back side.
        std::size_t 
        back_free() const 
        {
            return capacity()-back_;
        }

        //! <b>Returns<\b>: the number of bytes free from the front side.
        std::size_t 
        front_free() const 
        {
            return front_;
        }

        //! <b>Returns<\b>: the buffer's size.
        std::size_t 
        size() const 
        {
            return back_-front_;
        }

        //! <b>Returns<\b>: the buffer's capacity.
        std::size_t 
        capacity() const 
        {
            return capacity_;
        }

        //! <b>Returns<\b>: a pointer to the current payload. 
        const void* 
        data() const 
        {
            return data()+front_;
        }

        //! <b>Returns<\b>: a pointer to the front of the payload.
        //!
        //! <b>Preconditions<\b>: size() < size
        //! 
        //! <b>Throws<\b>: invalid_parameter if the payload size is less than the required size.
        const void* 
        peek_front_i(std::size_t size) const
        {
            return data()+front_;
        }
        //! <b>Returns<\b>: a pointer to the front of the payload.
        //!
        //! <b>Throws<\b>: invalid_parameter if the payload size is less than the required size.
        const void* 
        front(std::size_t size) const
        {
            if (size() < size) throw not_enough();
            return peek_front_i(size);
        }
        
        //! <b>Returns<\b>: a reference to the front of the payload as if it stored an object of type T.
        //! 
        //! <b>Throws<\b>: invalid_parameter if the payload size is less than the size of the type \c T.
        template <typename T>
        const T& 
        front() const 
        {
            return  *(reinterpret_cast<const T*>(front(sizeof(T))));
        }
        
        //! <b>Returns<\b>: a pointer to the back of the payload.
        //! 
        //! <b>Throws<\b>: invalid_parameter if the payload size is less than the required size.
        const void* 
        back(std::size_t size) const
        {
            if (size() < size) throw not_enough();
            return data()+back_-size;
        }

        //! <b>Returns<\b>: the back of the payload as if it stored an object of type T.
        //! 
        //! <b>Throws<\b>: invalid_parameter if the payload size is less than the size of the type \c T.
        template <typename T>
        const T& 
        back() const 
        {
            return *(reinterpret_cast<T*>(back(sizeof(T))));
        }
        //! <b>Effects<\b>: increase the back pointer by size
        //! 
        //! <b>Throws<\b>: invalid parameter if the back free bytes are less than size.
        //! 
        //!\param ptr the address pointing to the data to push.
        //!\param size the number of bytes to push.
        void 
        pop_back(std::size_t size) 
        {
            if (size>back_free()) throw override();
            back_ptr()-=size;
        }
        //! <b>Effects<\b> push a T from the buffer's back size
        //! 
        //!\param val: the T to push
        //! 
        //! <b>Throws<\b>: if sizeof(T)>back_free()
        /// 
        template <typename T>
        void 
        pop_back() 
        {
            discard_back(sizeof(T));
        }        
        void 
        pop_front(std::size_t size) 
        {
            if (size>front_free()) throw override();
            front_+=size;
        }
        
        //! <b>Effects<\b>: push a T from the buffer's front size
        //! 
        //! \param val:the T to push
        //! 
        //! <b>Throws<\b>: if sizeof(T)>front_free() 
        template <typename T>
        void 
        pop_front() 
        {
            discard_front(sizeof(T));
        }
        

        //! <b>Effects<\b>: increase the back pointer by size
        //! 
        //! <b>Returns<\b>: The old back pointer. 
        //! 
        //! <b>Throws<\b>: invalid parameter if the back free bytes are less than size.
        //! 
        //!\param ptr the address pointing to the data to push.
        //!\param size the number of bytes to push.
        void* 
        write_back(std::size_t size) 
        {
            if (size>back_free()) throw override();
            void* old_back = back_ptr(); 
            back_ptr()+=size;
            return old_back;
        }
        
        //! <b>Effects<\b>: push size bytes from ptr T back on the frame.
        //! 
        //! <b>Throws<\b>: invalid parameter if the back free bytes are less than size.
        //! 
        //!\param ptr the address pointing to the data to push.
        //!\param size the number of bytes to push.
        void 
        push_back(void* ptr, std::size_t size) 
        {
            memcpy(write_back(size), ptr, size);
        }
        //! <b>Effects<\b> push a T from the buffer's back size
        //! 
        //!\param val: the T to push
        //! 
        //! <b>Throws<\b>: if sizeof(T)>back_free()
        /// 
        template <typename T>
        void 
        push_back(T const& val) 
        {
            *(reinterpret_cast<T*>(write_back(sizeof(T)))) = val;
        }        

        template <typename T>
        T& 
        write_back() 
        {
            return *(reinterpret_cast<T*>(write_back(sizeof(T))));;
        }        

        void* 
        write_front(std::size_t size) 
        {
            if (size>front_free()) throw override();
            front_-=size;
            memcpy(data()+front_, ptr, size);
        }
        
        void 
        push_front(void* ptr, std::size_t size) 
        {
            memcpy(write_front(size), ptr, size);
        }
        
        //! <b>Effects<\b>: push a T from the buffer's front size
        //! 
        //! \param val:the T to push
        //! 
        //! <b>Throws<\b>: if sizeof(T)>front_free() 
        template <typename T>
        void 
        push_front(T const& val) 
        {
            *(reinterpret_cast<T*>(write_front(sizeof(T)))) = val;
        }
        template <typename T>
        T& 
        write_front() 
        {
            return *(reinterpret_cast<T*>(write_front(sizeof(T))));
        }
        const void* 
        read_back(std::size_t size) 
        {
            if (size() < size) throw not_enough();
            back_-=size;
            return data()+back_;
        }
        
        void 
        read_back(char* ptr, std::size_t size) 
        {
            memcpy(ptr, read_back(size), size);
        }
        
        template <typename T>
        T& 
        read_back() 
        {
            return *(reinterpret_cast<T*>(read_back(sizeof(T))));
        }        
        const void* 
        read_front(std::size_t size) 
        {
            if (size() < size) throw not_enough();
            front_+=size;
            return data()+front_-size;
        }
        void 
        read_front(char* ptr, std::size_t size) 
        {
            memcpy(ptr, read_front(size), size);
        }
        template <typename T>
        T& 
        read_front() 
        {
            return *(reinterpret_cast<T*>(read_front(sizeof(T))));
        }
        
    };
    
    template <typename T>
    frame& operator<<(frame& ofr, const T& rhs) 
    {
        ofr.push_back(rhs);
        return ofr;
    }
    template <typename T>
    frame& operator>>(frame& ifr, const T& rhs) 
    {
        rhs=if.read_front<T>();
        return ifr;
    }
    template <typename T>
    frame& operator<<=(const T& rhs, frame& ifr) 
    {
        rhs=ifr.read_back<T>();
        return ifr;
    }
    template <typename T>
    frame& operator>>=(const T& rhs, frame& ofr) 
    {
        ofr.push_front(rhs);
        return ofr;
    }
}
}

#endif
