/**
 * database.tcc - 
 * @author: Jonathan Beard
 * @version: Fri Jan  2 19:08:42 2015
 * 
 * Copyright 2015 Jonathan Beard
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _DATABASE_TCC_
#define _DATABASE_TCC_  1

#include "pointer.hpp"
#include "signal.hpp"
#include <cstddef>


namespace raft
{
   class kernel;
}

namespace Buffer
{

/**
 * DataBase - not quite the best name since we 
 * conjure up a relational database, but it is
 * literally the base for the Data structs.
 */
template < class T > struct DataBase 
{
   DataBase( const std::size_t max_cap ) : max_cap ( max_cap ),
                                           length_store( sizeof( T ) * max_cap ),
                                           length_signal( sizeof( T ) * max_cap ){}

   /** 
    * copyFrom - implement in all sub-structs to 
    * copy the buffer.  Might need to reinterpret
    * cast the other object or other type of cast
    * in order to get all the data members you wish
    * to copy.
    * @param   other - struct to be copied
    */
   virtual void copyFrom( DataBase< T > *other ) = 0;


   /**
    * setSourceKernel - set the source kernel 
    * so that an object using this buffer can
    * have access to it, these must be preserved
    * across copies.  Null kernel references 
    * will fail an assertion and exit the program.
    * @param    k - raft::kernel * const
    */
   inline void setSourceKernel( raft::kernel * const k )
   {
      assert( k != nullptr );
      src_kernel = k;
      return;
   }

   /**
    * setDestKernel - set the destination kernel 
    * so that an object using this buffer can
    * have access to it, these must be preserved
    * across copies.  Null kernel references 
    * will fail an assertion and exit the program.
    * @param    k - raft::kernel * const
    */
   inline void setDestKernel( raft::kernel * const k ) 
   {
      assert( k != nullptr );
      dst_kernel = k;
      return;
   }

   const std::size_t        max_cap;
   /** sizes, might need to define a local type **/
   const std::size_t       length_store;
   const std::size_t       length_signal;
   
   /** 
    * allocating these as structs gives a bit
    * more flexibility later in what to pass
    * along with the queue.  It'll be more 
    * efficient copy wise to pass extra items
    * in the signal, but conceivably there could
    * be a case for adding items in the store
    * as well.
    */
   Pointer                 *read_pt   = nullptr;
   Pointer                 *write_pt  = nullptr;
   
   
   T                       *store         = nullptr;
   Signal                  *signal        = nullptr;
   bool                    external_alloc = false;
   /** variable set by scheduler, used for shutdown **/
   bool                    is_valid       = true;


   /**
    * need a reference to source and destination 
    * kernels for preemption to work properly, essentially
    * there has to be a way to give orderly control from
    * the fifo copying data back to the scheduler and the
    * only realistic way is through the kernel.  These
    * must be preserved across copies.
    */
   raft::kernel            *src_kernel      = nullptr;
   raft::kernel            *dst_kernel      = nullptr;
};

} /** end namespace Buffer **/
#endif /* END _DATABASE_TCC_ */
