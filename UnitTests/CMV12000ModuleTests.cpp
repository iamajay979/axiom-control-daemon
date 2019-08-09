#include <catch/catch.hpp>

#include "../Modules/CMV12000Module.h"

TEST_CASE( "Digital gain setting", "[CMV12000Module]" ) //Tests SetConfigRegister and GetConfigRegister also indirectly
{
    CMV12000Module module;
    std::string message = "";
    std::string parameterValue = "";
    std::string emptyValue = "";

    SECTION( "Set Negative parameter value")
    {   
        parameterValue = "-1";
        CHECK( module.HandleParameter("set", "digital_gain", parameterValue, emptyValue, message) == false ); 
    }

    SECTION( "Set Out of bound value")
    {
        parameterValue = "10";
        CHECK( module.HandleParameter("set", "digital_gain", parameterValue, emptyValue, message) == false ); 
    }

    SECTION( "Set Digital Gain value")
    {
        parameterValue = "3";
        CHECK( module.HandleParameter("set", "digital_gain", parameterValue, emptyValue, message) == true ); 
    }

    SECTION( "Get Digital Gain")
    {
        REQUIRE( module.HandleParameter("get", "digital_gain", parameterValue, emptyValue, message) == true );
    }
}

TEST_CASE( "Analog gain setting", "[CMV12000Module]") //Tests SetConfigRegister and GetConfigRegister also indirectly
{
    CMV12000Module module;
    std::string message = "";
    std::string parameterValue = "";
    std::string emptyValue = "";

    SECTION( "Set Negative parameter value")
    {
        parameterValue = "-1";
        CHECK( module.HandleParameter("set", "analog_gain", parameterValue, emptyValue, message) == false ); 
    }

    SECTION( "Set out of bound value")
    {
        parameterValue = "8";
        CHECK( module.HandleParameter("set", "analog_gain", parameterValue, emptyValue, message) == false ); 
    }

    SECTION( "Set Analog Gain")
    {   
        parameterValue = "3";
        CHECK( module.HandleParameter("set", "analog_gain", parameterValue, emptyValue, message) == true ); 
    }

    SECTION( "Get Analog Gain")
    {
        CHECK( module.HandleParameter("get", "analog_gain", parameterValue, emptyValue, message) == true ); 
    }
}

TEST_CASE( "Dummy parameter test, test should succeed if SetParameter() returns false (no handler found)", "[CMV12000Module]" ) 
{
    CMV12000Module module;
    std::string message = "";
    std::string parameterValue = "3";
    std::string emptyValue = "";
    REQUIRE( module.HandleParameter("get", "dummyParameter", parameterValue, emptyValue, message) == false );
}

TEST_CASE( "Correct command test , only `set` and `get` parameters are allowed", "[CMV12000Module]" )
{
    CMV12000Module module;
    std::string message = "";
    std::string parameterValue = "3";
    std::string emptyValue = "";
    CHECK( module.HandleParameter("dummyCommand", "analog_gain", parameterValue, emptyValue, message) == false );
}

// TEST_CASE( "Test available methods retrieval", "[DaemonModule]" )
// {   
//     CMV12000Module module;
//     std::vector<std::string> availableMethods;// = module.GetAvailableMethods();
//     //REQUIRE(availableMethods.size() == 3);
//     //REQUIRE( responseMessage == expectedData );
// }
