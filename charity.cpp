
#include "charity.hpp"

namespace eosio
{


/// @abi action
void charity::hi(account_name user)
{
    print("Hello, ", name{user});
}

void charity::transfer(account_name from, account_name to, asset quantity, std::string memo)
{
    print("Receiving transfer message: from ", name{from}, " to ", name{to}, "\n");

    // define the table
    records trans_table(_self, from);

    // save the record.
    trans_table.emplace(_self, [&](auto &s) {
        s.id = trans_table.available_primary_key();
        s.from = from;
        s.to = to;
        s.quantity = quantity;
        s.memo = memo;
    });

}

} // namespace eosio


// modify form EOSIO_ABI
#define EOSIO_ABI_EX( TYPE, MEMBERS ) \
extern "C" { \
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
      auto self = receiver; \
      if( action == N(onerror)) { \
         /* onerror is only valid if it is for the "eosio" code account and authorized by "eosio"'s "active permission */ \
         eosio_assert(code == N(eosio), "onerror action's are only valid from the \"eosio\" system account"); \
      } \
      if( code == self || code == N(eosio.token) || action == N(onerror) ) { \
         TYPE thiscontract( self ); \
         switch( action ) { \
            EOSIO_API( TYPE, MEMBERS ) \
         } \
         /* does not allow destructor of thiscontract to run: eosio_exit(0); */ \
      } \
   } \
}

EOSIO_ABI_EX(eosio::charity, (hi)(transfer))
