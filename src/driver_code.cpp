#include <iostream>
#include <functional> // EXIT_SUCCESS
#include <cassert> // assert
#include <tuple>

#include "hash.hpp"

//! Accounts definition
struct Account{

   // typedef int AcctKey;
	using AcctKey = std::tuple < std::string, int , int , int > ;
/*-------------------------------- Public -----------------------------------*/
    
    std::string client_name;
    int bank_code;
    int branch_code;
    int acc_number;
    float balance;

/*------------------------------ Constructor --------------------------------*/
    
    Account( std::string n_ = "<empty>",
            int bnc_ = 1,       // Banc numeric code
            int brc_ = 1999,    // Agency
            int nmr_ = 0,       // Account Number
            float bal_ = 0.f )  // Account Balance
        : client_name( n_ ),
        bank_code( bnc_ ),
        branch_code( brc_ ),
        acc_number( nmr_ ),
        balance( bal_ )
    { /* Empty */ }

/*-------------------------------- Public -----------------------------------*/        
    
    //! @brief Returns the key related to the account
    AcctKey get_key()
    {
        return std::make_tuple(client_name,bank_code,branch_code, acc_number);
    } 

    //! @brief Prints an account 
    inline friend std::ostream &operator<< ( std::ostream & _os, const Account & _acc )
    {
        _os << "\n\tClient: < " << _acc.client_name <<
            " >\n\tBank: < "   << _acc.bank_code <<
            " >\n\tBranch: < " << _acc.branch_code <<
            " >\n\tNumber: < " << _acc.acc_number <<
            " >\n\tBalance: < " << _acc.balance << " >\n";
        return _os;
    }
};

//! 1st Hash Function.
struct KeyHash
{
    std::size_t operator()( const Account::AcctKey & k_ ) const
    {
       // std::cout << "\n\t>>> [KeyHash()]: key = " << std::hash< int >()( k_.acc_number ) << "\n";
        
        // Calculates a Hash value
        int ret_value =  std::hash< int >()( std::get<1>(k_)); // This part we can change
		for(char ch : std::get<0>(k_)){
			ret_value += ch;
		}
		ret_value += std::hash< int > () (std::get<2>(k_));
		ret_value += std::hash< int > () (std::get<3>(k_));
		
			return ret_value;
    }
};

//! Comparation Functor
struct KeyEqual
{
	bool operator()( const Account::AcctKey & _lhs, const Account::AcctKey & _rhs ) const
	{
		return  ( std::get<1>(_lhs)== std::get<1>(_rhs))
			and (std::get<2>(_lhs) == std::get<2>(_rhs) )
			and (std::get<3>(_lhs) == std::get<3>(_rhs) )
			and (std::get<0>(_lhs) == std::get<0>(_rhs) );
			
	}
};


int main()
{
    {
        //REMAKE
    	Account acct("Jack Jhonson", 1, 1999, 44876, 10000.f);

    	Account myAccounts[] =
            {
                {"Carla Bruni", 1, 1999, 44876, 10000.f},
                {"Elis Regina", 1, 1999, 45794, 0.f},
                {"Jorge Ben jor", 2, 557, 87629, 1050.f},
                {"Benito de Paula", 12, 331, 1231, 850.f},
                {"Hans Otto", 4, 666, 5, 530.f}
            };

        // Hash table with capacity = 23
    	HashTable< Account::AcctKey, Account, KeyHash, KeyEqual > contas(21);

        assert( contas.capacity() == 23);
        assert( contas.count() == 0 );

        contas.insert( myAccounts[2].get_key(), myAccounts[2] );
        contas.insert( myAccounts[0].get_key(), myAccounts[0] );
        contas.insert( myAccounts[3].get_key(), myAccounts[3] );
        contas.insert( myAccounts[1].get_key(), myAccounts[1] );
        contas.insert( myAccounts[4].get_key(), myAccounts[4] );

        // Printing Accounts
        std::cout << "                                                    Test Accounts" << "                                             \n";
        contas.print();
        assert( contas.count() == 5 );

        std::cout << "                                              Deleting Elis's Account..." << "                                             \n";
        if( contas.remove( myAccounts[1].get_key() ) )
            std::cout << "                                                Removed Successfully" << "                                               \n";
        else
            std::cout << "                                     Account not finded, Error while Deleting" << "                                   \n";

        contas.print();
        assert( contas.count() == 4 );

        Account acc1;
        contas.retrieve( myAccounts[3].get_key(),  acc1);
        assert( myAccounts[3].get_key() ==  acc1.get_key() );
        assert( myAccounts[3].client_name ==  acc1.client_name );
        assert( myAccounts[3].branch_code ==  acc1.branch_code );

        //myAccounts[2] was removed, retrieve needs to be false
        assert( contas.retrieve( myAccounts[1].get_key(),  acc1) == false ); 
    }

/*----------------------------- Rehash Testing ------------------------------*/ 
    {
        std::cout << "                                                  Testing ReHash..." << "                                             \n";
        Account accs[] = {

                {"Joyce", 1, 1999, 35412, 10000.f},
                {"Ramon", 1, 1999, 63526, 530.f},
                {"Elsa", 2, 557, 32133, 1000000.f},
                {"Raquel", 3, 331, 27840, 850.f},
                {"Miguel", 2, 666, 98766, 5490.f},
                {"Talita", 5, 666, 99999, 5490.f}
            };

        std::cout << "                                      Creating a Hash Table with  capacity = 2..." << "                                             \n";

        HashTable< Account::AcctKey, Account, KeyHash, KeyEqual > contas(2);

        assert( contas.capacity() == 2 );
        assert( contas.count() == 0 );

        std::cout << "                                                         DONE!" << "                                             \n";

        std::cout << "                                              Now Rehash must take action..." << "                                             \n";

        // More Accounts than it supports, to rehash
        contas.insert( accs[0].get_key(), accs[0] );
        contas.insert( accs[1].get_key(), accs[1] );
        contas.insert( accs[2].get_key(), accs[2] );
        contas.insert( accs[3].get_key(), accs[3] );
        contas.insert( accs[4].get_key(), accs[4] );
        contas.insert( accs[5].get_key(), accs[5] );
        contas.print();

        std::cout << "                                             Rehash worked Successfully!" << "                                             \n";

        assert( contas.count() == 6 );
    }

    std::cout << "\n\n>>> Exiting Successfully...\n";
	return EXIT_SUCCESS;
}
