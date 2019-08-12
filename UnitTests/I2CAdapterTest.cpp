#include <catch/catch.hpp>

#include "../Adapters/I2CAdapterDummy.h"

TEST_CASE( "I2C Get test", "[I2CAdapter]")
{
	I2CAdapterDummy adapter;

	//Random initialization
	std::string i2cbusArg = "1";
	std::string chipAddressArg = "0x28";
	std::string dataAddressArg = "0x11";
	std::string message = "";
	char mode = 'b';

	SECTION( "Out of bound Bus Address check")
	{	
		i2cbusArg = "-1";
		CHECK( adapter.I2cGet(i2cbusArg, chipAddressArg, dataAddressArg, mode, message) == -1); 
		i2cbusArg = "0x100000";
		CHECK( adapter.I2cGet(i2cbusArg, chipAddressArg, dataAddressArg, mode, message) == -1);
	}

	SECTION( "Out of bound Chip Address check")
	{
		chipAddressArg = "0x02";
		CHECK( adapter.I2cGet(i2cbusArg, chipAddressArg, dataAddressArg, mode, message) == -1); 
		chipAddressArg = "0x78";
		CHECK( adapter.I2cGet(i2cbusArg, chipAddressArg, dataAddressArg, mode, message) == -1); 
		chipAddressArg = "-1";
		CHECK( adapter.I2cGet(i2cbusArg, chipAddressArg, dataAddressArg, mode, message) == -1); 
	}

	SECTION( "Out of bound Data Address check")
	{	
		dataAddressArg = "-2";
		CHECK( adapter.I2cGet(i2cbusArg, chipAddressArg, dataAddressArg, mode, message) == -1); 
		dataAddressArg = "0x100";
		CHECK( adapter.I2cGet(i2cbusArg, chipAddressArg, dataAddressArg, mode, message) == -1); 
	}

	SECTION( "Getting I2C register value with Correct Parameters")
	{
		REQUIRE( adapter.I2cGet(i2cbusArg, chipAddressArg, dataAddressArg, mode, message) != -1);
	}
}


TEST_CASE( "I2C Set test", "[I2CAdapter]")
{
	I2CAdapterDummy adapter;

	//Random initialization
	std::string i2cbusArg = "1";
	std::string chipAddressArg = "0x28";
	std::string dataAddressArg = "0x11";
	std::string valueArg = "0x15";
	std::string message = "";
	char mode = 'b';

	SECTION( "Out of bound Bus Address check")
	{	
		i2cbusArg = "-1";
		CHECK( adapter.I2cSet(i2cbusArg, chipAddressArg, dataAddressArg, valueArg, mode, message) == -1); 
		i2cbusArg = "0x100000";
		CHECK( adapter.I2cSet(i2cbusArg, chipAddressArg, dataAddressArg, valueArg, mode, message) == -1);
	}

	SECTION( "Out of bound Chip Address check")
	{
		chipAddressArg = "0x02";
		CHECK( adapter.I2cSet(i2cbusArg, chipAddressArg, dataAddressArg, valueArg, mode, message) == -1); 
		chipAddressArg = "0x78";
		CHECK( adapter.I2cSet(i2cbusArg, chipAddressArg, dataAddressArg, valueArg, mode, message) == -1); 
		chipAddressArg = "-1";
		CHECK( adapter.I2cSet(i2cbusArg, chipAddressArg, dataAddressArg, valueArg, mode, message) == -1); 
	}

	SECTION( "Out of bound Data Address check")
	{	
		dataAddressArg = "-1";
		CHECK( adapter.I2cSet(i2cbusArg, chipAddressArg, dataAddressArg, valueArg, mode, message) == -1); 
		dataAddressArg = "0x100";
		CHECK( adapter.I2cSet(i2cbusArg, chipAddressArg, dataAddressArg, valueArg, mode, message) == -1); 
	}

	SECTION("Invalid Value to be set")
	{
		valueArg = "-1";
		CHECK( adapter.I2cSet(i2cbusArg, chipAddressArg, dataAddressArg, valueArg, mode, message) == -1); 
	}

	SECTION( "Byte Mode : Value out of bound")
	{
		valueArg = "0x100";
		CHECK( adapter.I2cSet(i2cbusArg, chipAddressArg, dataAddressArg, valueArg, mode, message) == -1); 
	}

	SECTION( "Word Mode : Value out of bound")
	{
		valueArg = "0x10000";
		mode = 'w';
		CHECK( adapter.I2cSet(i2cbusArg, chipAddressArg, dataAddressArg, valueArg, mode, message) == -1); 
	}

	SECTION( "Getting I2C register value with Correct Parameters")
	{	
		REQUIRE( adapter.I2cSet(i2cbusArg, chipAddressArg, dataAddressArg, valueArg, mode, message) != -1);
	}
}