//
// chat_join_timer.cpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <vector>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "./chat_defs.hpp"

typedef chat_chan::port port_t;

//timeout handler
void timeout_hdlr(boost::shared_ptr<void> p)
{
  cout << "recv a timeout event at choice !" << endl;
}
//chat msg handler; print out data
void choice_msg_hdlr1(boost::shared_ptr<void> p)
{
  chat_msg *msg = (chat_msg *) p.get();
  cout << "choice_msg_hdlr1 recved: [" << msg->data_ << "]" << endl;
}
void choice_msg_hdlr2(boost::shared_ptr<void> p)
{
  chat_msg *msg = (chat_msg *) p.get();
  cout << "choice_msg_hdlr2 recved: [" << msg->data_ << "]" << endl;
}

void join_msg_hdlr(std::vector<boost::shared_ptr<void> >& p)
{
  cout << "join_msg_hdlr recved " << p.size() << "msgs:\n";
  for(size_t i=0; i < p.size(); i++) {
    cout << "msg " << i << ":";
    chat_msg *msg = (chat_msg *) p[i].get();
    cout << " [" << msg->data_ << "]" << endl;
  }
}

class my_timer
{
public:
  my_timer(boost::asio::io_service& io, port_t &p)
    : timer_(io, boost::posix_time::seconds(5)),
      port_(p)
  {
    timer_.async_wait(boost::bind(&my_timer::timeout, this));
  }

  void timeout()
  {
    port_.send(new null_event); //send timeout notifications
    timer_.expires_at(timer_.expires_at() + boost::posix_time::seconds(5));
    timer_.async_wait(boost::bind(&my_timer::timeout, this));
  }

private:
  boost::asio::deadline_timer timer_;
  port_t &port_;
};

int main(int, char **) {
  //create executor with one thread
  exec_type exec(1);

  //create local channel and run all channel async operations inside executor
  chat_chan chan(&exec);

  //create/bind-to subjects i am going to speak about
  chat_chan::out basketball_o(chan, basketball);
  chat_chan::out tennis_o(chan, tennis);
  chat_chan::out baseball_o(chan, baseball);
  chat_chan::out tax_o(chan, tax);
  chat_chan::out stock_o(chan, stock);
  chat_chan::out investment_o(chan, investment);

  boost::asio::io_service io;
  port_t timeout_port;
  my_timer(io,timeout_port);
  //spawn a thread to run timer
  boost::thread t(boost::bind(&boost::asio::io_service::run, &io));

  //subscribe to sport msgs thru choice which will
  //fire callback mesg handlers when any mesg arrives
  choice_arbiter_t choice(chan);
  choice.bind(basketball, choice_msg_hdlr1);
  choice.bind(tennis, choice_msg_hdlr2);
  choice.bind(baseball, choice_msg_hdlr1);
  choice.bind(timeout_port, timeout_hdlr);

  //subscribe to financial msgs thru join which will
  //only fire the callback mesg handler when all subjects
  //have messages arriving
  join_arbiter_t join(chan, join_msg_hdlr);
  join.bind(tax);
  join.bind(stock);
  join.bind(investment);

  //start sending msgs and see how choice and join work differently
  char msg[1024];
  bool cont = true;
  while (cont) {
    //speak
    cout << "which subject : 1-basket_ball, 2-tennis, 3-baseball, 4-tax, 5-stock, 6-investment, 7-exit:\n";
    int subject;
    cin.getline(msg, 1024);
    subject = atoi(msg);
    switch(subject) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      cout << "enter your message: ";
      cin.getline(msg, 1024);
      switch (subject) {
      case 1:
	basketball_o.send(new chat_msg(msg));
	break;
      case 2:
	tennis_o.send(new chat_msg(msg));
	break;
      case 3:
	baseball_o.send(new chat_msg(msg));
	break;
      case 4:
	tax_o.send(new chat_msg(msg));
	break;
      case 5:
	stock_o.send(new chat_msg(msg));
	break;
      case 6:
	investment_o.send(new chat_msg(msg));
	break;
      default:
	break;
      }
      break;
    case 7:
      cont = false;
      break;
    default:
      break;
    }
  }

  exec.shut_down_wait();

  cout << "... exit ...\n";
  return 0;
}
