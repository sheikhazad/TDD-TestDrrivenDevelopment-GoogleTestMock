#ifndef SIMPLEORDERMANAGER_HPP_INCLUDED
#define SIMPLEORDERMANAGER_HPP_INCLUDED

class Order
{
	//represents an order
};

class OrderWriter
{
public:
	virtual void writeOrders(const std::vector<Order>& allOrders)
	{
		// ... writes the orders somewhere ...
	}
};

class OrderStore
{
public:
	virtual std::vector<Order> getOrders() const
	{
		// ... returns an array of orders ...
	}
};

class SimpleOrderManager
{
public:
	void writeAllOrders(OrderStore& orderStore, OrderWriter& orderWriter)
	{
		std::vector<Order> allOrders = orderStore.getOrders();

		if(allOrders.size() == 0)
			throw "No orders in store";
		orderWriter.writeOrders(allOrders);
	}
};


#endif // SIMPLEORDERMANAGER_HPP_INCLUDED
