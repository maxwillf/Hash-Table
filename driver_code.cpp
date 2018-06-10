#include <iostream>
#include <functional> // EXIT_SUCCESS
#include <cassert> // assert
#include <tuple>

#include "hash.hpp"

//! Accounts definition
struct Account{

    typedef int AcctKey;

/*-------------------------------- Public -----------------------------------*/
    
    std::string client_name;
    int bank_code;
    int branch_code;
    int acc_number;
    float balance;

/*------------------------------ Constructor --------------------------------*/
    
    Account( std::string n_ = "<empty>",
            int bnc_ = 1,       // Banco do Brasil
            int brc_ = 1668,    // Campus UFRN agency
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
        return acc_number;
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
        // This is just to debug.
        std::cout << "\n\t>>> [KeyHash()]: key = " << std::hash< int >()( k_ ) << std::endl;
        // Calcular o valore de dispersao.
        return std::hash< int >()( k_ );
    }
};

//! Comparation Functor
struct KeyEqual
{
	bool operator()( const Account::AcctKey & _lhs, const Account::AcctKey & _rhs ) const
	{
		return ( _lhs == _rhs );
	}
};


int main()
{
    {
        //REMAKE
    	Account acct("Alex Bastos", 1, 1668, 54321, 1500.f);

    	Account myAccounts[] =
            {
                {"Carla Bruni", 1, 1668, 54321, 1500.f},
                {"Elis Regina", 1, 1668, 45794, 530.f},
                {"Jorge Ben jor", 13, 557, 87629, 150000.f},
                {"Benito de Paula", 18, 331, 1231, 850.f},
                {"Hans Otto", 116, 666, 1, 5490.f}
            };

        // Hash tablee with capacity = 23
    	HashTable< Account::AcctKey, Account > contas(21);

        assert( contas.capacity() == 23 );
        assert( contas.count() == 0 );

        contas.insert( myAccounts[2].get_key(), myAccounts[2] );
        contas.insert( myAccounts[0].get_key(), myAccounts[0] );
        contas.insert( myAccounts[3].get_key(), myAccounts[3] );
        contas.insert( myAccounts[1].get_key(), myAccounts[1] );
        contas.insert( myAccounts[4].get_key(), myAccounts[4] );

    #ifdef _NOT_NOW
        // Em um segundo momento... [OK]
        Account conta1;

        contas.retrieve( myAccounts[2].get_key(), conta1 );
        std::cout << conta1.client_name << std::endl;
    #endif

        contas.print();
        assert( contas.count() == 5 );

        std::cout << "                                              Deleting Elis's Account..." << "                                             \n";
        if( contas.remove( myAccounts[1].get_key() ) )
            std::cout << "                                             Removed Successfully." << "                                               \n";
        else
            std::cout << "                                     Account not finded, Error while Deleting." << "                                   \n";

        contas.print();
        assert( contas.count() == 4 );

        Account acc1;
        contas.retrieve( myAccounts[3].get_key(),  acc1);
        assert( myAccounts[3].get_key() ==  acc1.get_key() );
        assert( myAccounts[3].client_name ==  acc1.client_name );
        assert( myAccounts[3].branch_code ==  acc1.branch_code );

        //myAccounts[2] ja foi removido de contas, entao o retrieve tem que ser false
        assert( contas.retrieve( myAccounts[1].get_key(),  acc1) == false ); 
    }

/*------------------------- Prime Numbers Testing ---------------------------*/ 
    {
        std::cout << "                                              Testing Prime Numbers..." << "                                             \n";

        assert( is_prime(13) );
        assert( is_prime(37) );
        assert( is_prime(59) );
        assert( is_prime(67) );
        assert( is_prime(83) );
        assert( is_prime(97) );
        assert( next_prime(9)  == 11 );
        assert( next_prime(55) == 59 );
        assert( next_prime(74) == 79 );
        assert( next_prime(38) == 41 );
    }

/*----------------------------- Rehash Testing ------------------------------*/ 
    {
        std::cout << "                                              Testing ReHash..." << "                                             \n";
        Account accs[] = {

                {"Joyce", 1, 1668, 35412, 1500.f},
                {"Ramon", 1, 1668, 93264, 530.f},
                {"Elsa", 13, 557, 12745, 150000.f},
                {"Raquel", 18, 331, 27840, 850.f},
                {"Miguel", 116, 666, 65423, 5490.f},
                {"Talita", 116, 666, 56789, 5490.f}
            };

        HashTable< Account::AcctKey, Account > contas(2);

        assert( contas.capacity() == 2 );
        assert( contas.count() == 0 );

        contas.insert( accs[0].get_key(), accs[0] );
        contas.insert( accs[1].get_key(), accs[1] );
        contas.insert( accs[2].get_key(), accs[2] );
        contas.insert( accs[3].get_key(), accs[3] );
        contas.insert( accs[4].get_key(), accs[4] );
        contas.insert( accs[5].get_key(), accs[5] );
        contas.print();

        assert( contas.capacity() == 5 );
        assert( contas.count() == 6 );
    }

    std::cout << "\n\n>>> Exiting Successfully...\n";
	return EXIT_SUCCESS;
}
