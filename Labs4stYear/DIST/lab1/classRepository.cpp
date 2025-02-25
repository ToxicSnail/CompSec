#include <iostream>
#include <typeinfo>

class AbstractEntity {
public:
    virtual ~AbstractEntity() = default;
};

class AccountEntity : public AbstractEntity 
{   
public:
    void Print() { std::cout << "AccountEntity: Action completed" << std::endl; }
};

class RoleEntity : public AbstractEntity 
{  
public:
    void Print() { std::cout << "RoleEntity: Action completed" << std::endl; }
};

class Repository 
{ 
public:
    virtual void Save(AbstractEntity* entity) = 0;
    virtual ~Repository() = default;
};

class AccountRepository : public Repository 
{
public:
    void Save(AbstractEntity* entity) override 
    {
        if (typeid(*entity) != typeid(AccountEntity)) 
        {
            std::cout << "//" << typeid( *entity ).name();
            std::cout << " Error: trying to save a non-AccountEntity in the AccountRepository" << std::endl;
            return;
        }
        std::cout << " Saving AccountEntity..." << std::endl;
    }
};

class RoleRepository : public Repository 
{
public:
    void Save(AbstractEntity* entity) override 
    {
        if (typeid(*entity) != typeid(RoleEntity)) 
        {
            std::cout << "//" << typeid( *entity ).name();
            std::cout << " Error: trying to save a non-RoleRepository in the RoleRepository" << std::endl;
            return;
        }
        std::cout << " Saving RoleEntity..." << std::endl;
    }
};

int main() 
{
    AccountEntity account;
    RoleEntity role;

    AccountRepository accountRepo;
    RoleRepository roleRepo;

    //account.Print();

    std::cout << " Positive tests" << std::endl;
    accountRepo.Save(&account);
    roleRepo.Save(&role);
    
    std::cout << "\n Negative tests" << std::endl;
    accountRepo.Save(&role);
    roleRepo.Save(&account);
    
    return 0;
}


/*
посмотреть как работает typeid
*/
