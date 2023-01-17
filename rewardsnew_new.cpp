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
  
  struct Reward {
      std::string reward_owner_account; 
	  std::string reward_owner_account_type; 
	  double reward_percentage; 
	  std::string reward_created_date;
	  std::string reward_modified_date; 
	  double payout_to_date; 
	  std::string ram_payer; 
	 };
Reward rewardtest ={0};
   

	struct [[eosio::table]] assets_s {
		uint64_t asset_id;
		double revenue_to_date;
		double platform_costs_to_date;
		double profit_to_date;
		double reward_percentage;
		double profit_target; 
		double current_reward; 
		std::string asset_owner;
		std::string account_asset_owner_type; 
        std::string asset_description;
        std::string ipfs_link; 
		name ram_payer;
		//std::vector<asset> backed_tokens;
		//std::vector<uint8_t> immutable_serialized_data;
		//std::vector<uint8_t> mutable_serialized_data;
		std::vector<Reward> rewardtest;
		uint64_t primary_key() const { return asset_id; }
	};


	typedef multi_index<"assetsnew"_n, assets_s> assets_tn;
    

   




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

	[[eosio::action]]
	void addassets( name user, std::string memo, double profit_target, double current_reward, double reward_percentage, std::string asset_owner, std::string account_asset_owner_type, name ram_payer, std::string asset_description, std::string ipfs_link)
	{
	//assuming this contract resides with realotuser11, check to verify auth to initiate action. 
	require_auth( get_self() );
	//emplace info into table

// Instantiate multi-index table aka: Get the first row
   // (1) Owner of table. (2) Account name this contract is deployed to
   //      get_self() gets the name of the contract 
	    assets_tn asset_(get_self(), get_first_receiver().value);
   /*
	   auto itr = asset_.begin();
while(itr != asset_.end()){
    itr = asset_.erase(itr);
}
*/

	//Insert New Asset Into Table
       asset_.emplace(get_self(), [&]( auto& row){
           row.asset_id = asset_.available_primary_key();
		   row.revenue_to_date = 0.00;
           row.platform_costs_to_date = 0.00;
		   row.profit_to_date = 0.00; 
		   row.reward_percentage = reward_percentage; 
		   row.profit_target = profit_target; 
		   row.current_reward = current_reward; 
		   row.asset_owner = "phoenix";
		   row.account_asset_owner_type = "DB";
           row.asset_description = asset_description;
           row.ipfs_link = ipfs_link;
		   row.ram_payer = ram_payer;
		  
       });


	} //end addmessage


	[[eosio::action]]
	void updateasset( std::uint8_t asset_id, double revenue_to_date, double platform_costs_to_date, name user, std::string memo, double profit_target, double profit_to_date, double current_reward, double reward_percentage, std::string asset_owner, std::string account_asset_owner_type, name ram_payer, std::string asset_description, std::string ipfs_link)
	{
	//assuming this contract resides with realotuser11, check to verify auth to initiate action. 
	require_auth( get_self() );
	//emplace info into table

    // Instantiate multi-index table aka: Get the first row
    // (1) Owner of table. (2) Account name this contract is deployed to
    // get_self() gets the name of the contract 
	    assets_tn asset_(get_self(), get_first_receiver().value);

    //Find Asset Based on Asset ID
        auto itr = asset_.find(asset_id);
      if ( itr == asset_.end()) {
       return; 
       }

	//Update Asset From Table
         asset_.modify( itr, _self, [&]( auto& row ){
		   row.revenue_to_date = (revenue_to_date > 0 && !(isnan(revenue_to_date))) ? revenue_to_date : itr->revenue_to_date;

           row.platform_costs_to_date = (platform_costs_to_date > 0 && !(isnan(platform_costs_to_date))) ? platform_costs_to_date : itr->platform_costs_to_date;

		   row.profit_to_date = (profit_to_date > 0 && !(isnan(profit_to_date))) ? profit_to_date : itr->profit_to_date; 

		   row.reward_percentage = (reward_percentage > 0 && !(isnan(reward_percentage))) ? reward_percentage : itr->reward_percentage;

		   row.profit_target = (profit_target > 0 && !(isnan(profit_target))) ? profit_target : itr->profit_target; 

		   row.current_reward = (current_reward > 0 && !(isnan(current_reward))) ? current_reward : itr->current_reward; 

		   row.asset_owner = (asset_owner.length() > 5) ? asset_owner : itr->asset_owner;

		   row.account_asset_owner_type = (account_asset_owner_type.length() > 2) ? asset_owner : itr->account_asset_owner_type;

           row.asset_description = (asset_description.length() > 5) ? asset_description : itr->asset_description;
           
           row.ipfs_link = (ipfs_link.length() > 5) ? ipfs_link: itr->ipfs_link;

		   row.ram_payer = (ram_payer.length() > 5) ? ram_payer : itr->ram_payer;
       });


	} //end updateasset


    /*
    addnewreward is an action that adds new reward record to a specific asset listed on the platform. It takes as parameter the asset_id, which is the primary key for the asset (or property of interest). It then also takes in the owner account name, the owner_account_type, the reward percentage allocation, the reward created date, modified date, payout and the name of the ram payer. 
    */

	[[eosio::action]]
	void addnewreward(std::uint8_t asset_id, std::string reward_owner_account,std::string reward_owner_account_type, double reward_percentage, std::string reward_created_date, std::string reward_modified_date, double payout_to_date, std::string ram_payer)
	{
	require_auth( get_self() );
	assets_tn asset_(get_self(), get_first_receiver().value);
    auto itr = asset_.find(asset_id);
    
    asset_.modify( itr, _self, [&]( auto& row ) {
        row.rewardtest.push_back(Reward());
        auto position = row.rewardtest.size() -1;
        row.rewardtest[position].reward_owner_account = reward_owner_account;
        row.rewardtest[position].reward_owner_account_type = reward_owner_account_type;
        row.rewardtest[position].reward_percentage = reward_percentage;
        row.rewardtest[position].reward_created_date = reward_created_date;
        row.rewardtest[position].reward_modified_date = reward_modified_date;
        row.rewardtest[position].payout_to_date = payout_to_date;
        row.rewardtest[position].ram_payer = ram_payer;
    });

	} //end addnewreward


	[[eosio::action]]
	void modifyreward( std::uint8_t asset_id, std::uint8_t reward_id,std::string reward_owner_account,std::string reward_owner_account_type, double reward_percentage, std::string reward_created_date, std::string reward_modified_date, double payout_to_date, std::string ram_payer)
	{
	require_auth( get_self() );
	assets_tn asset_(get_self(), get_first_receiver().value);
    auto itr = asset_.find(asset_id);
    
    asset_.modify( itr, _self, [&]( auto& row ) {
       
        row.rewardtest[reward_id].reward_owner_account = reward_owner_account;
        row.rewardtest[reward_id].reward_owner_account_type = reward_owner_account_type;
        row.rewardtest[reward_id].reward_percentage = reward_percentage;
        row.rewardtest[reward_id].reward_created_date = reward_created_date;
        row.rewardtest[reward_id].reward_modified_date = reward_modified_date;
        row.rewardtest[reward_id].payout_to_date = payout_to_date;
        row.rewardtest[reward_id].ram_payer = ram_payer;
    });

	}


	[[eosio::action]]
	void deletereward( std::uint8_t asset_id, std::uint8_t reward_id)
	{
	require_auth( get_self() );
	assets_tn asset_(get_self(), get_first_receiver().value);
    auto itr = asset_.find(asset_id);
    
    asset_.modify( itr, _self, [&]( auto& row ) { 
    row.rewardtest.erase(row.rewardtest.begin());
    });
	}




[[eosio::action]]
	void deleteasset( uint8_t asset_id)
	{
	//assuming this contract resides with realotuser11, check to verify auth to initiate action. 
	require_auth( get_self() );
	assets_tn asset_(get_self(), get_first_receiver().value);

    auto itr = asset_.find(asset_id);
    
    if ( itr == asset_.end() ) {
     printf("Record does not exist in table, nothing to delete");
     return;
    }
     asset_.erase( itr );

	} //end deleteasset


[[eosio::action]]
	void deleteassets()
	{
	//assuming this contract resides with realotuser11, check to verify auth to initiate action. 
	require_auth( get_self());
	assets_tn asset_(get_self(), get_first_receiver().value);

	auto itr = asset_.begin();
    
    while(itr != asset_.end()){
    itr = asset_.erase(itr);
    }
    
	} //end  deleteassets

























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
