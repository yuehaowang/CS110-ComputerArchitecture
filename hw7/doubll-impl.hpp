///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// __detail::__List_node_base///// ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


// default constructor of node base
template <typename _Tp>
__detail::__List_node_base<_Tp>::__List_node_base () : _M_data()
{
    // init prev node to null
    _M_prev = nullptr;
    // init next node to null
    _M_next = nullptr;
}
// copy constructor of node base
template <typename _Tp>
__detail::__List_node_base<_Tp>::__List_node_base (const _Tp &value)
{
    // save data
    _M_data = value;

    // init prev node to null
    _M_prev = nullptr;
    // init next node to null
    _M_next = nullptr;
}


///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// __detail::__List_const_iterator ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

template <typename _Tp>
class __detail::__List_const_iterator
{
public:
    // a pointer save address of corresponding node
    __List_node_base<_Tp> *p;

    // no default constructor
    __List_const_iterator() : p() { }

    // convert iterator to const iterator
    __List_const_iterator ( const __List_iterator<_Tp> &other )
    {
        // copy node pointer
        p = other.p;
    }

    // copy constructor
    __List_const_iterator (const __List_const_iterator<_Tp> &other)
    {
        // copy node pointer
        p = other.p;
    }

    // init with a node
    explicit __List_const_iterator (__List_node_base<_Tp> *node)
    {
        // save the node pointer
        p = node;
    }

    __List_const_iterator<_Tp> &operator= (const __List_const_iterator<_Tp> &rhs)
    {
        p = rhs.p;

        return *this;
    }

    // dereference overload
    const _Tp& operator* () const
    {
        // get saved data in the node
        return p->_M_data;
    }

    // arrow overload
    const _Tp* operator-> () const
    {
        // return address of saved data in node
        return &(p->_M_data);
    }

    // prefix self increment overload
    __List_const_iterator<_Tp> &operator++ ()
    {
        // jump to the next
        p = p->_M_next;
        // return self
        return *this;
    }

    // postfix self increment overload
    __List_const_iterator<_Tp> operator++ (int)
    {
        // save old iterator
        __List_const_iterator<_Tp> temp = *this;

        // jump to the next
        p = p->_M_next;

        // return the old iterator
        return temp;
    }

    // pretfix self increment overload
    __List_const_iterator<_Tp> &operator-- ()
    {
        // jump to the prev
        p = p->_M_prev;
        // return self
        return *this;
    }

    // postfix self decrement overload
    __List_const_iterator<_Tp> operator-- (int)
    {
        // save old iterator
        __List_const_iterator<_Tp> temp = *this;

        // jump to the prev
        p = p->_M_prev;

        // return the old iterator
        return temp;
    }

    // equal overload
    bool operator== (const __List_const_iterator<_Tp>& other) const
    {
        // compare their saved node's address
        return p == other.p;
    }

    // not equal overload
    bool operator!= (const __List_const_iterator<_Tp>& other) const
    {
        // inverse of equal
        return !(*this == other);
    }
};

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// __detail::__List_iterator /////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

template <typename _Tp>
class __detail::__List_iterator
{
public:
    __List_node_base<_Tp> *p;

    __List_iterator () : p(nullptr) { }

    __List_iterator (const __List_iterator<_Tp> &other)
    {
        p = other.p;
    }

    explicit __List_iterator (__List_node_base<_Tp> *node)
    {
        p = node;
    }

    __List_iterator<_Tp> &operator= (const __List_iterator<_Tp> &rhs)
    {
        p = rhs.p;

        return *this;
    }

    _Tp& operator* () const
    {
        return p->_M_data;
    }

    _Tp* operator-> () const
    {
        return &(p->_M_data);
    }

    __List_iterator<_Tp> &operator++ ()
    {
        p = p->_M_next;

        return *this;
    }

    __List_iterator<_Tp> operator++ (int)
    {
        __List_iterator<_Tp> temp = *this;

        p = p->_M_next;

        return temp;
    }

    __List_iterator<_Tp> &operator-- ()
    {
        p = p->_M_prev;

        return *this;
    }

    __List_iterator<_Tp> operator-- (int)
    {
        __List_iterator<_Tp> temp = *this;

        p = p->_M_prev;

        return temp;
    }

    bool operator== (const __List_iterator<_Tp>& other) const
    {
        return p == other.p;
    }

    bool operator!= (const __List_iterator<_Tp>& other) const
    {
        return !(*this == other);
    }
};

///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// doubll /////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

template <typename _Tp>
doubll<_Tp>::doubll() : _M_size(0)
{
    _M_head._M_next = &_M_tail;
    _M_head._M_prev = nullptr;
    _M_tail._M_prev = &_M_head;
    _M_tail._M_next = nullptr;
}

template <typename _Tp>
doubll<_Tp>::doubll(typename doubll<_Tp>::size_type size, const _Tp& value) : _M_size(0)
{
    _M_head._M_next = &_M_tail;
    _M_head._M_prev = nullptr;
    _M_tail._M_prev = &_M_head;
    _M_tail._M_next = nullptr;

    // replicate node
    for (size_type i = 0; i < size; i++) {
        insert(end(), value);
    }
}

template <typename _Tp>
doubll<_Tp>::~doubll()
{
    iterator it = begin();

    // traverse the whole linked list and delete every node
    while (it != end()) {
        it = erase(it);
    }
}

template <typename _Tp>
typename doubll<_Tp>::size_type doubll<_Tp>::size () const
{
    return _M_size;
}

template <typename _Tp>
typename doubll<_Tp>::iterator doubll<_Tp>::begin ()
{
    return iterator(_M_head._M_next);
}

template <typename _Tp>
typename doubll<_Tp>::iterator doubll<_Tp>::end ()
{
    return iterator(&_M_tail);
}

template <typename _Tp>
typename doubll<_Tp>::const_iterator doubll<_Tp>::cbegin () const
{
    return const_iterator(_M_head._M_next);
}

template <typename _Tp>
typename doubll<_Tp>::const_iterator doubll<_Tp>::cend () const
{
    return const_iterator(const_cast<__detail::__List_node_base<_Tp>*>(&_M_tail));
}

template <typename _Tp>
typename doubll<_Tp>::iterator doubll<_Tp>::insert (typename doubll<_Tp>::iterator pos, const _Tp &value)
{
    __detail::__List_node_base<_Tp> *new_node = new __detail::__List_node_base<_Tp>(value);
    
    new_node->_M_next = pos.p;
    new_node->_M_prev = pos.p->_M_prev;
    pos.p->_M_prev->_M_next = new_node;
    pos.p->_M_prev = new_node;

    ++_M_size;

    return iterator(new_node);
}

template <typename _Tp>
typename doubll<_Tp>::iterator doubll<_Tp>::insert (typename doubll<_Tp>::const_iterator pos, const _Tp &value)
{
    __detail::__List_node_base<_Tp> *new_node = new __detail::__List_node_base<_Tp>(value);
    
    // set the next and prev of the new node
    new_node->_M_next = pos.p;
    new_node->_M_prev = pos.p->_M_prev;
    // reset prev's next and next's prev
    pos.p->_M_prev->_M_next = new_node;
    pos.p->_M_prev = new_node;

    ++_M_size;

    return iterator(new_node);
}

template <typename _Tp>
typename doubll<_Tp>::iterator doubll<_Tp>::erase (typename doubll<_Tp>::iterator pos)
{
    __detail::__List_node_base<_Tp> *next_node = pos.p->_M_next;

    pos.p->_M_next->_M_prev = pos.p->_M_prev;
    pos.p->_M_prev->_M_next = pos.p->_M_next;

    delete pos.p;

    --_M_size;

    return iterator(next_node);
}

template <typename _Tp>
typename doubll<_Tp>::iterator doubll<_Tp>::erase (typename doubll<_Tp>::const_iterator pos)
{
    __detail::__List_node_base<_Tp> *next_node = pos.p->_M_next;

    // kick self out of the linked list
    pos.p->_M_next->_M_prev = pos.p->_M_prev;
    pos.p->_M_prev->_M_next = pos.p->_M_next;
    
    // release memory
    delete pos.p;

    --_M_size;

    return iterator(next_node);
}

/// method:
/// 1. take the first node as n1
/// 2. insert it at the end
/// 3. take the first node as n2 (the second one in the original list)
/// 4. insert it at the n1
/// 5. loop 1-4 operations for n3 n4 ...
template <typename _Tp>
void doubll<_Tp>::reverse ()
{
    if (size() <= 1) {
        return;
    }

    // the number of processed node
    size_type n;
    // position to insert the temp node
    iterator it_pos = end();

    for (n = 0; n < size(); n++) {
        iterator it_temp = begin();

        // update the first node
        it_temp.p->_M_next->_M_prev = it_temp.p->_M_prev;
        it_temp.p->_M_prev->_M_next = it_temp.p->_M_next;

        // put the temp node to the reverse position
        it_temp.p->_M_next = it_pos.p;
        it_temp.p->_M_prev = it_pos.p->_M_prev;
        it_pos.p->_M_prev->_M_next = it_temp.p;
        it_pos.p->_M_prev = it_temp.p;

        // process next node
        it_pos = it_temp;
    }
}
