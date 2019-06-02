#include <catch/catch.hpp>

#include "../Modules/CMV12000Module.h"

TEST_CASE( "Analog gain setting", "[CMV12000Module]" )
{
    CMV12000Module module;
    std::string message = "";
    std::string parameterValue = "3";
    std::string emptyValue = "";
    CHECK( module.HandleParameter("set", "analog_gain", parameterValue, emptyValue, message) == true );
    REQUIRE( module.HandleParameter("get", "analog_gain", parameterValue, emptyValue, message) == true );
}

TEST_CASE( "Digital gain setting", "[CMV12000Module]" )
{
    CMV12000Module module;
    std::string message = "";
    std::string parameterValue = "3";
    std::string emptyValue = "";
    CHECK( module.HandleParameter("set", "digital_gain", parameterValue, emptyValue, message) == true );
    REQUIRE( module.HandleParameter("get", "digital_gain", parameterValue, emptyValue, message) == true );
}

TEST_CASE( "Dummy parameter test, test should succeed if SetParameter() returns false (no handler found)", "[CMV12000Module]" ) 
{
    CMV12000Module module;
    std::string message = "";
    std::string parameterValue = "3";
    std::string emptyValue = "";
    REQUIRE( module.HandleParameter("get", "dummyParameter", parameterValue, emptyValue, message) == false );
}

TEST_CASE( "Test available methods retrieval", "[DaemonModule]" )
{
    CMV12000Module module;

    std::vector<std::string> availableMethods;// = module.GetAvailableMethods();

    //REQUIRE(availableMethods.size() == 3);
    //REQUIRE( responseMessage == expectedData );
}
