#include <stdio.h>
#include "portexception.hpp"
#include "kernel.hpp"

using namespace raft;

std::size_t kernel::kernel_count( 0 );

/** default **/
kernel::kernel() : kernel_id( kernel::kernel_count )
{
   kernel::kernel_count++;
}

/** existing memory **/
kernel::kernel( void * const ptr,
                const std::size_t nbytes ) :
   input(  this, ptr, nbytes ),
   output( this, ptr, nbytes ),
   kernel_id( kernel::kernel_count )
{
}


std::size_t
kernel::get_id()
{
   return( kernel_id );
}

raft::kernel&
kernel::operator []( const std::string &&portname )
{
   if( enabled_port.size() < 2 )
   {
        enabled_port.push( portname );
   }
   else
   {
        throw AmbiguousPortAssignmentException(
            "too many ports added with: " + portname
        );
   }
   return( (*this) );
}

std::size_t
kernel::addPort()
{
   return( 0 );
}

void
kernel::lock()
{
   /** does nothing, just need a base impl **/
   return;
}

void
kernel::unlock()
{
   /** does nothing, just need a base impl **/
   return;
}

std::string
kernel::getEnabledPort()
{
    if( enabled_port.size() == 0 )
    {
        return( "" );
    }
    const std::string head( enabled_port.front() );
    enabled_port.pop();
    return( head );
}

//std::string
//kernel::getName()
//{
//   int status( 0 );
//   const std::string name_a(
//      abi::__cxa_demangle( typeid( *(this) ).name(), 0, 0, &status ) );
//
//}
