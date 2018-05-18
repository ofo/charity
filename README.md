# charity
一个简单的EOS智能合约例子，演示：

1. 转账时消息通知、处理机制
2. 持久化存储

有两点值得参考：
1. 修改EOSIO_ABI，使其能够处理转发过来的通知消息。
````C++
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
````

2. 使用自增主键
````c++
    trans_table.emplace(_self, [&](auto &s) {
        s.id = trans_table.available_primary_key();
        s.from = from;
        s.to = to;
        s.quantity = quantity;
        s.memo = memo;
    });

````
