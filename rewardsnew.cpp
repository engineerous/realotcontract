#include <eosio/eosio.hpp>
#include <eosio/print.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
#include <eosio/symbol.hpp>
#include <string>
#include <eosio/crypto.hpp>
#include <eosio/transaction.hpp>



using namespace eosio;


class [[eosio::contract("realottester")]] realottester : public eosio::contract {
	

public:
	using contract::contract;
	realottester(name receiver, name code, datastream<const char *> ds):contract(receiver, code, ds){}

	struct [[eosio::table]] assets_s {
		uint64_t asset_id;
		double revenue_to_date;
		double platform_costs_to_date;
		double profit_to_date;
		double reward_percentage;
		double profit_target; 
		double current_reward; 
		name collection_name;
		name schema_name;
		int32_t template_id;
		name ram_payer;
		//std::vector<asset> backed_tokens;
		//std::vector<uint8_t> immutable_serialized_data;
		//std::vector<uint8_t> mutable_serialized_data;
		uint64_t primary_key() const { return asset_id; }
	};


	typedef multi_index<"assets"_n, assets_s> assets_t;
    


//https://medium.com/coinmonks/advanced-eos-series-part-9-payable-actions-42bf878bee36
	[[eosio::on_notify("eosio.token::transfer")]] 
	void wegotpaid(name from, name to, eosio::asset quantity, std::string memo) {

		const eosio::name REAL_CONTRACT = "eosio.token"_n;

		const eosio::name ORIGINAL_CONTRACT = get_first_receiver();	

		/*
		Function get_first_reciever is function that provides 
		The account the incoming action was first received at.
		*/

		const std::string_view eosString{"EOS"};

		const uint8_t eosdecimals = 4;

		const symbol eossymbol(

			eosString,

			eosdecimals

		);

		const std::string_view realotdaoString{"RLT"};

		const uint8_t realotdaodecimals = 3;

		const symbol realotdaosymbol(

			realotdaoString,

			realotdaodecimals

		);

		check( quantity.amount > 1, "Quanity must be greater than 1 EOS" );

		check( REAL_CONTRACT == ORIGINAL_CONTRACT, "You tryna get over on us, bro?" );

		if( from != eosio::name("realottester") ){ return; }
		//if( from == get_self() || to != get_self() ){ return; }

		check( quantity.symbol == eossymbol, "Symbol is not what we were expecting" );

		//emplace info into table

		bal_table bals( get_self(), get_self().value );

		auto itr = bals.find( from.value );

		if( itr != bals.end() ){

			//modify this user's entry

			bals.modify( itr, same_payer, [&](auto &row) {

				row.balance += quantity;

			});

		} else {

			bals.emplace( get_self(), [&](auto &row) {

				row.user = from.value;

				row.balance = quantity;

			});

		}
/*
action(
        //permission_level -> permission_level{eosio::name("realotreward"), "active"_n}, 
        //code (token) -> , "realottester"_n
        //action -> "transfer"_n,
        //data -> std::tuple(from, owner, asset{quantity, symbol}, std::string(""))
      ); 

*/

         
		action(permission_level{eosio::name("realotuser11"), "active"_n}, "eosio.token"_n,
        "transfer"_n , std::tuple{ eosio::name("realotuser11"), from, asset(10000, eossymbol), std::string("We just reacted son")}        ).send();

	
	
	
	
	}//end of on_notify

/*


	[[eosio::on_notify("atomicassets::transfer")]] 
	void listen(name from, name to, std::vector<uint64_t> &ids, std::string memo) {

		const eosio::name REAL_CONTRACT = "atomicassets"_n;

		const eosio::name ORIGINAL_CONTRACT = get_first_receiver();	

		check( REAL_CONTRACT == ORIGINAL_CONTRACT, "You tryna get over on us, bro?" );

		if( from == get_self() || to != get_self() ){ return; }

		check( ids.size() >= 1, "You must send at least 1 NFT" );

		atomics_t assetidtbl( "atomicassets"_n, get_self().value );

		for( auto THIS_ASSET: ids ){

			auto asset_itr = assetidtbl.find(THIS_ASSET);

			check(asset_itr != assetidtbl.end(), "Could not locate this asset");

			burn_asset(THIS_ASSET);	

		}
	



	}//end atomicassets transfer

*/
/*
uint64_t asset_id;
		uint64_t revenue_to_date;
		uint64_t platform_costs_to_date;
		uint64_t profit_to_date;
		uint16_t reward_percentage;
		uint16_t profit_target; 
		uint16_t current_reward; 
		name collection_name;
		name schema_name;
		int32_t template_id;
		name ram_payer;
		std::vector<asset> backed_tokens;
		std::vector<uint8_t> immutable_serialized_data;
		std::vector<uint8_t> mutable_serialized_data;
		uint64_t primary_key() const { return asset_id; 
*/
	[[eosio::action]]
	void addasset( name user, std::string memo, double profit_target, double current_reward, double reward_percentage, name collection_name, name schema_name, int32_t template_id, name ram_payer)
	{
	//assuming this contract resides with realotuser11, check to verify auth to initiate action. 
	require_auth( get_self() );
	//emplace info into table

// Instantiate multi-index table aka: Get the first row
   // (1) Owner of table. (2) Account name this contract is deployed to
   //      get_self() gets the name of the contract 
	    assets_t asset_(get_self(), get_first_receiver().value);
   
	   auto itr = asset_.begin();
while(itr != asset_.end()){
    itr = asset_.erase(itr);
}


/*
	//Insert New Asset Into Table
       asset_.emplace(get_self(), [&]( auto& row){
           row.asset_id = asset_.available_primary_key();
		   row.revenue_to_date = 0.00;
           row.platform_costs_to_date = 0.00;
		   row.profit_to_date = 0.00; 
		   row.reward_percentage = reward_percentage; 
		   row.profit_target = profit_target; 
		   row.current_reward = current_reward; 
		   row.collection_name = collection_name;
		   row.schema_name = schema_name;
		   row.template_id = 25; 
		   row.ram_payer = ram_payer;

       });



*/





/*
	msg_table msgs( get_self(), get_self().value );

	msgs.emplace( user, [&](auto &row) {

			row.ID = msgs.available_primary_key();

			row.message = message;

		});
		*/

	} //end addmessage


	[[eosio::action]]
	void adduserrecd( name user )
	{
		require_auth( get_self() );

		//emplace info into table

		balsss_table usertable( get_self(), get_self().value );

		usertable.emplace( get_self(), [&](auto &row) {

			row.ID = usertable.available_primary_key();

			row.username = user;

			row.total_records = 0;

		});

	} //end addmessage



	[[eosio::action]]
	void countrecordm( name user )
	{
		require_auth( get_self() );

		//emplace info into table

		balsss_table usertable( get_self(), get_self().value );

		auto itr = usertable.find(2);

		check( itr != usertable.end(), "Row 2 was not found" );

		auto users_secondary = usertable.get_index<"username"_n>();

		auto users_low_itr = users_secondary.lower_bound("realottester"_n.value);

		auto users_up_itr = users_secondary.upper_bound("realottester"_n.value);

		int foundCount = 0;

		for( auto found_itr = users_low_itr; found_itr != users_up_itr; found_itr ++ ){

			foundCount ++;

		}


		usertable.modify( itr, get_self(), [&](auto &row) {

			row.total_records = foundCount;

		});

	} //end addmessage


	

//END OF PUBLIC

private:

    //burn an asset
	
	/*
    void burn_asset(uint64_t assetID){
      action(
	      permission_level{get_self(), "active"_n},
	      "atomicassets"_n,
	      "burnasset"_n,
	      std::tuple{ get_self(), assetID }
  	  ).send();
  	}
	*/

	struct [[eosio::table]] messages {

		uint64_t ID;

		std::string message; 

		uint64_t primary_key() const { return ID; }

	};

	using msg_table = eosio::multi_index<"messages"_n, messages
	>;


	struct [[eosio::table]] balances {

		uint64_t user;

		eosio::asset balance;

		uint64_t primary_key() const { return user; }

	};

	using bal_table = eosio::multi_index<"balances"_n, balances
	>;


	struct [[eosio::table]] balancesss {

		uint64_t ID;

		eosio::name username;

		eosio::asset balance;

		uint64_t total_records;

		uint64_t primary_key() const { return ID; }

		uint64_t second_key() const { return username.value; }

	};

	using balsss_table = eosio::multi_index<"balancesss"_n, balancesss,
	eosio::indexed_by<"username"_n, eosio::const_mem_fun<balancesss, uint64_t, &balancesss::second_key>>
	>;



//END OF PRIVATE


	};
