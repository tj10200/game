#ifndef TVector_h
#define TVector_h

namespace containers
{
    template < typename TClassType, typename TSizeType >
    class TVector
    {
        public:

            /**
             * Default Constructor
             */
            TVector();

            /**
             * Constructor with initializer
             *
             * @param a_size - the initial size of the array
             * @param a_growSize - the size to grow by
             */
            TVector ( TSizeType a_size,
                      TSizeType a_growSize );

            /**
             * Destructor
             */
            ~TVector();

            /**
             * Initializer for the array. Will resize the array
             * if already created
             *
             * @param a_size - the size to make the array.
             * @param a_growSize - the size to grow the array on resize
             */
            void initialize ( TSizeType a_size,
                              TSizeType a_growSize );

            /**
             * Clear the items in the vector
             */
            void clear();

            /**
             * visitor function used to act on the vector elements
             *
             * @parma a_func - the function to call
             * @parma a_data - the data to pass to the function
             */
            template <typename TFunc, typename TData >
            void visit ( TFunc a_func, TData a_data );

            /**
             * find function used to return single item from the list
             *
             * @parma a_index - the find index
             * @param ar_obj - will be populated with found result
             * @return bool - true if found
             */
            bool find ( TSizeType a_index, TClassType& ar_type );

            /**
             * Push an item to the back of the array 
             *
             * @param a_item - the item to insert
             */
            void insert ( TClassType a_item );

            /**
             * Operator[] returns value at index
             *
             * @param a_index - the index to return
             * @return const TClassType - the object
             */
            const TClassType& operator [] ( int i );

            /**
             * The number of elements currently stored
             *
             * @return TSizeType - the number of elements
             */
            TSizeType size();

            /**
             * The max size of the array before the next resize
             *
             * @return TSizeType - the max num of elements
             */
            TSizeType maxSize();

            /**
             * Gets the grow size 
             *
             * @return TSizeType - the grow size
             */
            TSizeType growSize();

        private:

            /** The array of objects being held **/
            TClassType* mp_array;
        
            /** The size of the array **/
            TSizeType m_arraySize;

            /** The array grow size **/
            TSizeType m_growSize;

            /** The number of elements being stored **/
            TSizeType m_numItems;
    };

    //-----------------------------------------------------------------------//
    template < typename TClassType, typename TSizeType >
    TVector< TClassType, TSizeType >::TVector()
    : mp_array ( NULL ),
      m_arraySize ( 0 ),
      m_growSize ( 5 ),
      m_numItems ( 0 )
    {}

    //-----------------------------------------------------------------------//
    template < typename TClassType, typename TSizeType >
    TVector< TClassType, TSizeType >::TVector ( TSizeType a_size,
                                                TSizeType a_growSize )
    {
        initialize ( a_size, a_growSize );
    }

    //-----------------------------------------------------------------------//
    template < typename TClassType, typename TSizeType >
    TVector < TClassType, TSizeType >::~TVector()
    {
        if ( NULL != mp_array )
        {
            delete [] mp_array;
        }
    }

    //-----------------------------------------------------------------------//
    template < typename TClassType, typename TSizeType >
    void TVector < TClassType, TSizeType >::initialize ( TSizeType a_size,
                                                         TSizeType a_growSize )
    {
        if ( 0 == m_arraySize )
        {
            mp_array = new TClassType [ a_size ];
        }
        else
        {
            if ( m_arraySize < a_size )
            {
                TClassType* lp_newArray = new TClassType[ a_size ];
                
                m_arraySize = a_size;

                for ( int i = 0; i < m_numItems; ++i )
                {
                    mp_array[i] = lp_newArray[i];
                }
                
                delete [] mp_array;
                mp_array = lp_newArray;
            }
        }

        m_growSize = a_growSize;
    }

    //-----------------------------------------------------------------------//
    template < typename TClassType, typename TSizeType >
    void TVector < TClassType, TSizeType >::clear()
    {
        //Lazy clearing of the vector. 
        //The items are no longer directly accessible
        m_numItems = 0;
    }

    //-----------------------------------------------------------------------//
    template < typename TClassType, typename TSizeType >
    template <typename TFunc, typename TData >
    void TVector < TClassType, TSizeType >::visit ( TFunc a_func, TData a_data )
    {
        for ( int i = 0; i < m_numItems; ++i )
        {
            (*a_func) ( mp_array[i], a_data );
        }
    }

    //-----------------------------------------------------------------------//
    template < typename TClassType, typename TSizeType >
    bool TVector < TClassType, TSizeType >::find ( TSizeType a_index, TClassType& ar_obj )
    {
        bool l_ret = true;

        if ( a_index < m_numItems )
        {
            ar_obj = mp_array[a_index];
        }
        else
        {
            l_ret = false;
        }

        return l_ret;
    }

    //-----------------------------------------------------------------------//
    template < typename TClassType, typename TSizeType >
    void TVector < TClassType, TSizeType >::insert ( TClassType a_item )
    {
        if ( m_numItems == m_arraySize )
        {
            initialize ( m_arraySize + m_growSize, m_growSize );
        }

        mp_array [ m_numItems++ ] = a_item;
    }

    //-----------------------------------------------------------------------//
    template < typename TClassType, typename TSizeType >
    const TClassType& TVector < TClassType, TSizeType >::operator []( int a_index )
    {
        return mp_array[a_index];
    }

    //-----------------------------------------------------------------------//
    template < typename TClassType, typename TSizeType >
    TSizeType TVector < TClassType, TSizeType >::size()
    {
        return m_numItems;
    }

    //-----------------------------------------------------------------------//
    template < typename TClassType, typename TSizeType >
    TSizeType TVector < TClassType, TSizeType >::maxSize()
    {
        return m_arraySize;
    }
            
    //-----------------------------------------------------------------------//
    template < typename TClassType, typename TSizeType >
    TSizeType TVector < TClassType, TSizeType >::growSize()
    {
        return m_growSize;
    }
};

#endif
