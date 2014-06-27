#include "json/json.h"
#include "gtest/gtest.h"
#include <fstream>

TEST (SimpleJsonTest, Parsing)
{
    Json::Value l_rootNode; 
    Json::Reader l_reader;
    std::ifstream l_testFile ( "jsonchecker/testframework.json", std::ifstream::binary);

    bool l_parsingSuccessful = l_reader.parse( l_testFile, l_rootNode );
    
    ASSERT_TRUE ( l_parsingSuccessful );
    EXPECT_EQ ( l_rootNode[ "aNumber" ], 12345 );

    Json::Value l_array = l_rootNode["anArray"];

    EXPECT_EQ ( l_array[(Json::Value::UInt)0], 111 );
    EXPECT_EQ ( l_array[(Json::Value::UInt)1], 222 );
    EXPECT_EQ ( l_array[(Json::Value::UInt)2], 333 );

    Json::Value l_complex = l_rootNode["aComplexType"];
    
    EXPECT_TRUE ( true == l_complex["first"].asBool() );
    EXPECT_EQ ( l_complex["first"], l_rootNode["aComplexType"]["first"] );
    EXPECT_TRUE ( false == l_complex["second"].asBool() );
    EXPECT_EQ ( l_complex["second"], l_rootNode["aComplexType"]["second"] );

    EXPECT_EQ ( l_rootNode["NotANode"].asString(), ""); 
    EXPECT_EQ ( l_rootNode["NotANode"].asBool(), 0); 
    EXPECT_EQ ( l_rootNode["NotANode"].asInt(), 0); 
}
