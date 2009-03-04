void initiator() {
    frame mine;

#if REFERENCE_CLOSURE
    
    closure child;
    child.func = nested; child.scope = &mine;
    
#elif STD_FUNCTION

    lambda child;
    child.scope = &mine;
    
#endif 
    passer( child );
}
