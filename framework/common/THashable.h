#ifndef THashable_h
#define THashable_h

#include <stdint.h>
#include <stddef.h>

namespace framework
{
    template < typename T >
    class THashable
    {
        public:
            /**
             * Constructor
             */
            THashable() 
            : m_hashCode ( 0 ),
              m_hashComputed ( false )
            {}

            /**
             * Destructor
             */
            virtual ~THashable()
            {}

            /**
             * Setter for hte hash code
             */
            void setHashCode ( T a_hash )
            {
                m_hashCode = a_hash;
                m_hashComputed = true;
            }

            /**
             * Getter for the hash code
             */
            const T getHashCode() const
            {
                return m_hashCode;
            }

            /**
             * Hasher
             */
            virtual void computeHash() =0;

            /**
             * Setter for the hash computed flag
             */
            void setHashComputed ( bool a_flag )
            {
                m_hashComputed = a_flag;
            }

            /**
             * Getter for the hash computed flag
             */
            bool isHashComputed () const
            {
                return m_hashComputed;
            }

            /**
             * Hash struct
             */
            struct SHash
            {
                size_t operator() ( const THashable* const & apr_one ) const
                {
                    return apr_one->getHashCode();
                }
            };
            
            /**
             * Comparison Function
             */
            virtual bool equals ( const THashable* const & apr_two ) const
            {
                return getHashCode() == apr_two->getHashCode();
            }
            
            /**
             * Comparison Struct
             */
            struct SEquals
            {
                bool operator() ( const THashable* const & apr_one,
                                  const THashable* const & apr_two ) const
                {
                    return apr_one->equals ( apr_two );
                }
            };

            /**
             * LessThan Function
             */
            virtual bool less ( const THashable* const & apr_two ) const
            {
                return getHashCode() < apr_two->getHashCode();
            }

            /**
             * Less Than Struct
             */
            struct SLess
            {
                bool operator () ( const THashable* const & apr_one,
                                   const THashable* const & apr_two ) const
                {
                    return apr_one->less ( apr_two );
                }
            };


        private:

            /** The Hash code **/
            T m_hashCode;

            /** Has the hash been calculated **/
            bool m_hashComputed;
    };
};

#endif
