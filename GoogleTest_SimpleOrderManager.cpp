#include<gmock/gmock.h>
#include<gtest/gtest.h>
#include<vector>
#include<gmock/gmock-matchers.h>
#include "SimpleOrderManager.hpp"

using namespace std;
using ::testing::Return;
using ::testing::_;

/******************* Tests for SimpleOrderManager**********************************
1. Test if exception "No orders in store" is thrown when vector size is zero.
2. Test if no exception is thrown when vector size is not zero.
3. Test if method OrderWriter::writeOrders() is called.

You cannot:
•	Change the SimpleOrderManager in any way.
•	Change the dependencies in any way.
**********************************************************************************/

/////////////////////////////////////////////////////////////////////////////////////
//Need to mock class OrderStore as we need to return vector
//without changing original class OrderStore which is not implemented (thus not returning vector)
class MockOrderStore : public OrderStore
{
public:
	MOCK_CONST_METHOD0(getOrders, std::vector<Order> () );
};

//To test if OrderWriter::writeOrders() is called
class MockOrderWriter : public OrderWriter
{
public:
	MOCK_METHOD1(writeOrders, void (const std::vector<Order>& ) );
};

//Test Fixtures
class SimpleOrderManagerFixture : public testing::Test
{
public:

    std::vector<Order> vecOrder; //Empty uninitialised Vector for exception_test
    MockOrderStore  aMockOrderStore;
    OrderWriter aOrderWriter; //not MockOrderWriter
    SimpleOrderManager aSMO;
};

/////////////////////////////////////////////////////////////////////////////////////
//Test-1: Test if exception "No orders in store" is thrown when vector size is zero.
/////////////////////////////////////////////////////////////////////////////////////
TEST_F(SimpleOrderManagerFixture,  exception_test )
{
    //Call MockOrderStore::getOrders() not OrderStore::getOrders()
    //as we need to return vector without changing original class OrderStore
    EXPECT_CALL(aMockOrderStore, getOrders).Times(1).WillOnce(Return(vecOrder));

    //Using EXPECT_THROW(aSMO.writeAllOrders(aMockOrderStore, orderWriter), const char*);
    //Will just check if exception is thrown but will not match exact message.
    try{
        aSMO.writeAllOrders(aMockOrderStore, aOrderWriter);
    }
    catch(const char* ex){
        cout<< "LOG: Exception thrown : " << ex << endl;
        EXPECT_STREQ(ex,  std::string("No orders in store").c_str());
    }
    // Check that the wrong exception type is not thrown:
    catch(...) {
        FAIL() << "Expected 'No orders in store'";
    }
}

///////////////////////////////////////////////////////////////////////
//Test-2: Test if no exception is thrown when vector size is not zero.
///////////////////////////////////////////////////////////////////////
TEST_F(SimpleOrderManagerFixture,  no_exception_test )
{
    vecOrder.emplace_back(Order()); //Non-zero size vector

    //Call MockOrderStore::getOrders() not OrderStore::getOrders()
    //as we need to return vector without changing original class OrderStore
    EXPECT_CALL(aMockOrderStore, getOrders).Times(1).WillOnce(Return(vecOrder));
    EXPECT_NO_THROW(aSMO.writeAllOrders(aMockOrderStore, aOrderWriter));
}

///////////////////////////////////////////////////////////////////////
//Test-3: Test if method OrderWriter::writeOrders() is called.
///////////////////////////////////////////////////////////////////////
TEST_F(SimpleOrderManagerFixture,  expect_call_writeOrders )
{
    MockOrderWriter aMockOrderWriter;
    vecOrder.emplace_back(Order()); //Non-zero size vector

    //Call MockOrderStore::getOrders() not OrderStore::getOrders()
    //as we need to return vector without changing original class OrderStore
    EXPECT_CALL(aMockOrderStore, getOrders).Times(1).WillOnce(Return(vecOrder));
    EXPECT_CALL(aMockOrderWriter, writeOrders(_)).Times(1);
    aSMO.writeAllOrders(aMockOrderStore, aMockOrderWriter);
}

int main(int argc, char** argv)
{
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
