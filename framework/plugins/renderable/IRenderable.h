#ifndef IRenderable_h
#define IRenderable_h

#include "IPluggable.h"
#include <GL/freeglut.h>
#include "THashable.h"
#include <vector>
#include "TVector.h"

namespace Json
{
    class Value;
};

namespace framework
{
    class IRenderable: public IPluggable, THashable<uint32_t>
    {
        public:
            /**
             * Constructor
             *
             * @param ap_name - the name of the plugin
             * @parma a_id - the plugin ID
             */
            IRenderable(const char* ap_name,
                        const uint16_t a_id  );

            /**
             * Destructor
             */
            virtual ~IRenderable();

            /**
             * Renders the object
             */
            virtual void render() =0;

            /**
             * Inherited from base class
             */
            virtual void computeHash();

            /**
             * Compares the names of the objects
             */
            virtual bool equals ( const THashable* const & apr_two ) const;

            /**
             * Updates the renderable item
             *
             * @param ar_elapsedTime - the program elapsed time in seconds
             */
            virtual void update ( const float& ar_elapsedTime );

            /**
             * Adds a pre child to the list
             *
             * @param ap_child - the child object to add
             */
            void addPreChild ( IRenderable* ap_child );

            /**
             * Gets a pre child using the ID. If multiple
             * will return the first instance matching the ID
             *
             * @param a_id - the id to search for
             * @return IRenderable - the pointer to the child object
             */
            IRenderable* getPreChild ( uint32_t a_id );

            /**
             * Removes a child from the pre list
             *
             * @param a_id - the id of the child to remove
             * @return bool - true if found and destroyed
             */
            bool removePreChild ( uint32_t a_id );

            /**
             * Adds a post child to the list
             *
             * @param ap_child - the child object to add
             */
            void addPostChild ( IRenderable* ap_child );

            /**
             * Gets a post child using the ID. If multiple
             * will return the first instance matching the ID
             *
             * @param a_id - the id to search for
             * @return IRenderable - the pointer to the child object
             */
            IRenderable* getPostChild ( uint32_t a_id );

            /**
             * Removes a child from the post list
             *
             * @param a_id - the id of the child to remove
             * @return bool - true if found and destroyed
             */
            bool removePostChild ( uint32_t a_id );

            /**
             * Visitor updator. Updates all children
             *
             * @param ap_inst - the object to act on
             * @param ar_data - the data to act on
             */
            static void sVisitorUpdate ( IRenderable* ap_obj, const float& ar_data );

            /**
             * Visitor find. Finds the child matching the id
             *
             * @param ap_inst - the object to look up
             * @param apr_data - the object to return
             */
            static bool sVisitorFind ( IRenderable* ap_obj, uint32_t a_id, IRenderable*& apr_return );

            /**
             * Sets the static directory to use when looking for
             * shader files
             *
             * @param ar_dir - the shader dir
             */
            void setShaderDir ( std::string& ar_dir );
        
        protected:
            typedef std::vector< GLuint > tShaderVec;

            /**
             * Struct containing data about the shaders being used
             */
            struct SShaderData
            {
                std::string m_vertexShader;
                std::string m_fragmentShader;
            };
            
            /**
             * Loads shaders from the config 
             *
             * @param ar_rootNode - the node containing the shader files
             * @return bool - true if loaded and compiled successfully
             */
            bool loadShaders ( Json::Value& ar_rootNode );

            /**
             * Loads the shader code from a file,
             * and uses it to create the shader
             *
             * @param a_shaderType - the shader type
             * @param ar_shaderFile - the shader file
             * @return GLuint - the shader handle
             */
            GLuint loadShader ( GLenum a_shaderType,
                                std::string& ar_shaderFile );
            
            /**
             * Create Shader
             *
             * @param a_shaderType - the shader type
             * @param a_shaderCode - the shader function
             * @return GLuint - the shader handle
             */
            GLuint createShader ( GLenum a_shaderType,
                                  std::string& ar_shaderCode );

            
            /**
             * Creates a shader program
             *
             * @param ar_shaderVector& - the shader program vector
             * @return GLuint - the program handle
             */
            GLuint createProgram ( tShaderVec& ar_shaderVector );

            /**
             * Gets the virtex shader name
             *
             * @param ar_data - the data to populate
             */
            virtual void getVertextShaderName( SShaderData& ar_data ) = 0;

        protected:

            /** The vertex buffer handle **/
            GLuint  m_vertexBufferHandle;

            /** The children to render before this object **/
            containers::TVector < IRenderable*, uint32_t > m_preChildren;
            
            /** The children to render after this object **/
            containers::TVector < IRenderable*, uint32_t > m_postChildren;

            /** The Shader Program **/
            GLuint m_shaderProgramHandle;

            /** The shader dir shared across all IRenderable objects **/
            static std::string ms_shaderDir;
             
    };
};

#endif
