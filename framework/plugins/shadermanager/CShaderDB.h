#ifndef CShaderDB_h
#define CShaderDB_h

#include <string>
#include <vector>
#include <unordered_map>

#include "log4cxx/logger.h"
#include "GL/freeglut.h"

namespace framework
{
    class CShaderDB
    {
        public: 

            struct SShaderEntry
            {
                SShaderEntry& operator = ( const SShaderEntry& ar_other )
                {
                    m_shaderName = ar_other.m_shaderName;
                    m_shaderTypes.clear();
                    m_shaderTypes.reserve( ar_other.m_shaderTypes.size() );
                    m_shaderFiles.clear();
                    m_shaderFiles.reserve( ar_other.m_shaderFiles.size() );

                    for ( int i = 0; i < ar_other.m_shaderTypes.size(); ++i )
                    {
                        m_shaderTypes[i] = ar_other.m_shaderTypes[i];
                        m_shaderFiles[i] = ar_other.m_shaderFiles[i];
                    }
                }
                
                std::string m_shaderName;
                std::vector< GLenum > m_shaderTypes;
                std::vector< std::string > m_shaderFiles;
            };


            /**
             * Constructor
             *
             */
            CShaderDB ();

            /**
             * Destructor
             */
            ~CShaderDB();

            /**
             * Initialize the shader db
             *
             * @param a_file - the file to load the shader data from
             * @return bool - true on successful load
             */
            bool initialize ( std::string a_file );

            /**
             * Getter for the shader entry
             *
             * @param ap_name - the name of the entry
             * @param apr_entry - the entry to return
             * @return bool - was the entry found
             */
            bool getShaderEntry ( const std::string& ar_name, 
                                  SShaderEntry*& apr_entry );

            /**
             * Getter for the shader entry
             *
             * @param a_id - the entry id
             * @param apr_entry - the entry to return
             * @return bool - was the entry found
             */
            bool getShaderEntry ( const uint16_t a_id, 
                                  const SShaderEntry*& apr_entry );

        private:
            /** The array of shader strings **/
            std::vector < SShaderEntry > m_shaderDB; 

            /** The hash map that provides fast lookup using the shader name **/
            std::unordered_map < std::string, uint16_t > m_shaderDBIndex;

            /** The logger object **/
            log4cxx::LoggerPtr m_logger;

    };
};

#endif
