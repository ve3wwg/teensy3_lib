// signals.h by christoph @ forum.prjc.com
// Thu Apr 24 21:19:39 2014

#ifndef _SIGNALS_H_
#define _SIGNALS_H_

#include <list>
// #include <utility>

/** Interface for delegates with a specific set of arguments **/
template<typename... args>
class AbstractDelegate
{
  public:
    virtual void operator()(args&&...) const = 0;
    virtual ~AbstractDelegate() {}
};

/** Concrete member function delegate that discards the function's return value **/
template<typename T, typename ReturnType, typename... args>
class ObjDelegate : public AbstractDelegate<args...>
{
  public:
    /** member function typedef **/
    typedef ReturnType (T::*ObjMemFn)(args...);

    /** constructor **/
    ObjDelegate(T& obj, ObjMemFn memFn)
      : obj_(obj),
      memFn_(memFn)
    {
    }

    /** call operator that calls the stored function on the stored object **/
    void operator()(args&&... a) const
    {
      (obj_.*memFn_)(std::forward<args>(a)...);
    }

  private:
    /** reference to the object **/
    T& obj_;
    /** member function pointer **/
    const ObjMemFn memFn_;
};

/** Concrete function delegate that discards the function's return value **/
template<typename ReturnType, typename... args>
class FnDelegate : public AbstractDelegate<args...>
{
  public:
    /** member function typedef **/
    typedef ReturnType (*Fn)(args...);

    /** constructor **/
    FnDelegate(Fn fn)
      : fn_(fn)
    {
    }

    /** call operator that calls the stored function **/
    void operator()(args&&... a) const
    {
      (*fn_)(std::forward<args>(a)...);
    }

  private:
    /** function pointer **/
    const Fn fn_;
};

/** forward declaration **/
template<typename... args>
class Connection;

/** Signal class that can be connected to**/
template<typename... args>
class Signal
{
  public:
    /** connection pointer typedef **/
    typedef Connection<args...>* connection_p;

    /** constructor **/
    Signal()
      : connections_(NULL)
      {
      }

    /** call operator that calls all connected delegates.
      The most recently connected delegate will be called first **/
    void operator()(args&&... a) const
    {
      auto c = connections_;
      while(c != NULL)
      {
        (c->delegate())(std::forward<args>(a)...);
        c = c->next();
      }
    }

    /** connect to this signal **/
    void connect(connection_p p)
    {
      p->next_ = connections_;
      connections_ = p;
      p->signal_ = this;
    }

    /** disconnect from this signal.
      Invalidates the connection's signal pointer
      and removes the connection from the list **/
    void disconnect(connection_p conn)
    {
      // find connection and remove it from the list
      connection_p c = connections_;
      if (c == conn)
      {
//        std::cout << "signal::disconnect(): removing head" << std::endl;
        connections_ = connections_->next();
        conn->next_ = NULL;
        conn->signal_ = NULL;
        return;
      }
      while(c != NULL)
      {
        if (c->next() == conn)
        {
//          std::cout << "signal::disconnect(): removing in between" << std::endl;
          c->next_ = conn->next();
          conn->next_ = NULL;
          conn->signal_ = NULL;
          return;
        }
        c = c->next();
      }
    }

    /** destructor. disconnects all connections **/
    ~Signal()
    {
//      std::cout << "~Signal()" << std::endl;
      connection_p p = connections_;
      while(p != NULL)
      {
//        std::cout << "~Signal(): disconnecting connection in destructor" << std::endl;
        connection_p n = p->next();
        disconnect(p);
        p = n;
      }
    }

  private:
    connection_p connections_;
};

/** connection class that can be connected to a signal **/
template<typename... args>
class Connection
{
  public:
    /** template constructor for non-static member functions.
      allocates a new delegate on the heap **/
    template<typename T, typename ReturnType>
    Connection(Signal<args...>* signal, T& obj, ReturnType (T::*memFn)(args...))
      : delegate_(new ObjDelegate<T, ReturnType, args...>(obj, memFn)),
      signal_(NULL),
      next_(NULL)
    {
      signal->connect(this);
    }

    /** template constructor for static member functions and free functions.
      allocates a new delegate on the heap **/
    template<typename ReturnType>
    Connection(Signal<args...>* signal, ReturnType (*Fn)(args...))
      : delegate_(new FnDelegate<ReturnType, args...>(Fn)),
      signal_(NULL),
      next_(NULL)
    {
      signal->connect(this);
    }

    /** get reference to this connection's delegate **/
    AbstractDelegate<args...>& delegate() const
    {
      return *delegate_;
    }

    /** get pointer to next connection in the signal's list **/
    Connection* next() const
    {
      return next_;
    }

    /** is this connection connected to a valid signal? **/
    bool connected() const
    {
      return (signal_ != NULL);
    }

    /** desctructor. If the signal is still alive, disconnects from it **/
    ~Connection()
    {
//      std::cout << "~Connection()" << std::endl;
      if (signal_ != NULL)
      {
//        std::cout << "~Connection(): disconnecting from signal in destructor" << std::endl;
        signal_->disconnect(this);
      }
      delete delegate_;
    }

    friend class Signal<args...>;
  private:
    AbstractDelegate<args...>* delegate_;
    Signal<args...>* signal_;
    Connection* next_;
};

// free connect function: creates a connection (non-static member function) on the heap that can be used anonymously

template<typename T, typename ReturnType, typename... args>
Connection<args...>* connect(Signal<args...> *signal, T& obj, ReturnType (T::*memFn)(args...)) {

	auto p = new Connection<args...>(signal, obj, memFn);
	return p;
}

// free connect function: creates a connection (static member or free function) on the heap that can be used anonymously

template<typename ReturnType, typename... args>
Connection<args...>* connect(Signal<args...> *signal, ReturnType (*fn)(args...)) {

	auto p = new Connection<args...>(signal, fn);
	return p;
}

#endif // _SIGNALS_H_
