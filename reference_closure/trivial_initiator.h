void initiator() {
    frame mine; closure child;
    child.func = nested; child.scope = &mine;
    passer( child );
}
