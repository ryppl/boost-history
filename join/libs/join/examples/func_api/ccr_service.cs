/// Copyright Microsoft
///
/// Copied from Microsoft MSDN CCR User Guide
///
/// <summary>
/// Base type for all service messages. Defines a response PortSet used
/// by all message types.
/// </summary>
public class ServiceOperation
{
    public PortSet<string, Exception> ResponsePort = new PortSet<string, Exception>();
}

public class Stop : ServiceOperation
{
}

public class UpdateState : ServiceOperation
{
    public string State;
}

public class GetState : ServiceOperation
{
}

/// <summary>
/// PortSet that defines which messages the services listens to
/// </summary>
public class ServicePort : PortSet<Stop, UpdateState, GetState>
{
}
/// <summary>
/// Simple example of a CCR component that uses a PortSet to abstract
/// its API for message passing
/// </summary>
public class SimpleService
{
    ServicePort _mainPort;
    DispatcherQueue _taskQueue;
    string _state;

    public static ServicePort Create(DispatcherQueue taskQueue)
    {
        SimpleService service = new SimpleService(taskQueue);
        service.Initialize();
        return service._mainPort;
    }

    private void Initialize()
    {
        // using the supplied taskQueue for scheduling, activate three
        // persisted receivers, that will run concurrently to each other,
        // one for each item type
        Arbiter.Activate(_taskQueue,
            Arbiter.Receive<UpdateState>(true, _mainPort, UpdateHandler),
            Arbiter.Receive<GetState>(true, _mainPort, GetStateHandler)
        );
    }

    private SimpleService(DispatcherQueue taskQueue)
    {
        // create PortSet instance used by external callers to post items
        _mainPort = new ServicePort();

        // cache dispatcher queue used to schedule tasks
        _taskQueue = taskQueue;
    }
    void GetStateHandler(GetState get)
    {
        if (_state == null)
        {
            // To demonstrate a failure response,
            // when state is null will post an exception
            get.ResponsePort.Post(new InvalidOperationException());
            return;
        }

        // return the state as a message on the response port
        get.ResponsePort.Post(_state);
    }
    void UpdateHandler(UpdateState update)
    {
        // update state from field in the message
        _state = update.State;

        // as success result, post the state itself
        update.ResponsePort.Post(_state);
    }
}

In the example above we show a class implementing the common CCR pattern for a software component:

    * Definitions of message types used to interact with the component
    * Definition of a PortSet derived class that accepts the message types defined. Its not necessary to derive from PortSet, but its a convenient way to reuse a PortSet with a particular number of types.
    * A static Create method that initializes an instance of the component and returns a PortSet instance used to communicate with the component instance
    * A private Initialize method that attaches some Arbiters on the public PortSet external code will use to talk to the service

If no concurrency constraints exist between the different handlers, simple, persisted single item receivers can be used.
