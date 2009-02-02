// Document/View sample for Boost.Signals
// Copyright Keith MacDonald 2005.
//
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// For more information, see http://www.boost.org

#include <iostream>
#include <string>
#include <boost/signals2/signal.hpp>
#include <boost/bind.hpp>

class Document
{
public:
    typedef boost::signals2::signal<void ()>  signal_t;

public:
    Document()
    {}

    /* Connect a slot to the signal which will be emitted whenever
      text is appended to the document. */
    boost::signals2::connection connect(const signal_t::slot_type &subscriber)
    {
        return m_sig.connect(subscriber);
    }

    void append(const char* s)
    {
        m_text += s;
        m_sig();
    }

    const std::string& getText() const
    {
        return m_text;
    }

private:
    signal_t    m_sig;
    std::string m_text;
};

class View
{
public:
    View(Document& m)
        : m_document(m)
    {
        m_connection = m_document.connect(boost::bind(&View::refresh, this));
    }

    virtual ~View()
    {
        m_connection.disconnect();
    }

    virtual void refresh() const = 0;

protected:
    Document&               m_document;

private:
    boost::signals2::connection  m_connection;
};

class TextView : public View
{
public:
    TextView(Document& doc)
        : View(doc)
    {}

    virtual void refresh() const
    {
        std::cout << "TextView: " << m_document.getText() << std::endl;
    }
};

class HexView : public View
{
public:
    HexView(Document& doc)
        : View(doc)
    {}

    virtual void refresh() const
    {
        const std::string&  s = m_document.getText();

        std::cout << "HexView:";

        for (std::string::const_iterator it = s.begin(); it != s.end(); ++it)
            std::cout << ' ' << std::hex << static_cast<int>(*it);

        std::cout << std::endl;
    }
};

int main(int argc, char* argv[])
{
    Document    doc;
    TextView    v1(doc);
    HexView     v2(doc);

    doc.append(argc == 2 ? argv[1] : "Hello world!");
    return 0;
}
