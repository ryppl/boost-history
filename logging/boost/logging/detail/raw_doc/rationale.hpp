namespace boost { namespace logging {

/** 
@page rationale Rationales




@section rationale_init_logs Rationale for not forcing an init_logs() function

I could have used an init_logs() function which would be called first time a log is used. However, that would bring its own problems to the table:
How do I know the first time a log is used? \n
What if the logs are used too soon (and init_logs would be called too soon, before we can safely initialize the logs)? \n
What if we're using the logs in a multi-threaded environment, how do we guard for the init_logs to be called only once (how costly will it turn out)? \n
What about using it from DLL/EXE?



@section rationale_caching Rationale for - when caching - to use a mutex

When caching messages before logs are initialized, I internally use a mutex for adding each message to the cache.
I use this because:
- I assume there won't be too many (<1000), thus the impact on speed should be minimal
- this will be applied only while cache is turned off
- I do prefer safety over efficiency
- easier to implement this scenario








*/

}}
