#include "shaderbase.h"

ShaderBase::ShaderBase() :
    loaded(false)
{
    //ctor
}

ShaderBase::~ShaderBase()
{
    //dtor
    unload();
}

void ShaderBase::load(std::string vertex_shader, std::string fragment_shader)
{
    if (loaded) unload();

    // Init the shader program
    GLuint vertexshader = initshaders(GL_VERTEX_SHADER, vertex_shader.c_str()) ;
    GLuint fragmentshader = initshaders(GL_FRAGMENT_SHADER, fragment_shader.c_str()) ;
    program_id = initprogram(vertexshader, fragmentshader) ;

    // set loaded-status
    loaded = true;

}

void ShaderBase::unload()
{
    // deleting the shader object
    if (loaded)
    {
        // delete the program
        glDeleteProgram(program_id);

        loaded = false;
    }
}

void ShaderBase::switchTo()
{
    if (loaded)
    {
        glUseProgram(program_id);
    }
    else std::cerr << "Tried to use shader program before it was loaded\n";
}

void ShaderBase::validate()
{
    GLint validate_ok;
    glValidateProgram(program_id);
    glGetProgramiv(program_id, GL_VALIDATE_STATUS, &validate_ok);
    if (!validate_ok)
    {
        fprintf(stderr, "glValidateProgram:");
        //print_log(getProgramID());
    }
}

GLuint ShaderBase::getProgramID()
{
    return program_id;
}

bool ShaderBase::isLoaded()
{
    return loaded;
}

std::string ShaderBase::textFileRead (const char * filename)
{
    std::string str, ret = "" ;
    std::ifstream in ;
    in.open(filename) ;
    if (in.is_open())
    {
        getline (in, str) ;
        while (in)
        {
            ret += str + "\n" ;
            getline (in, str) ;
        }
        //    cout << "Shader below\n" << ret << "\n" ;
        return ret ;
    }
    else
    {
        std::cerr << "Unable to Open File " << filename << "\n" ;
        throw 2 ;
    }
}

void ShaderBase::programerrors (const GLint program)
{
    GLint length ;
    GLchar * log ;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length) ;
    log = new GLchar[length+1] ;
    glGetProgramInfoLog(program, length, &length, log) ;
    std::cerr << "Compile Error, Log Below\n" << log << "\n" ;
    delete [] log ;
}
void ShaderBase::shadererrors (const GLint shader)
{
    GLint length ;
    GLchar * log ;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length) ;
    log = new GLchar[length+1] ;
    glGetShaderInfoLog(shader, length, &length, log) ;
    std::cerr << "Compile Error, Log Below\n" << log << "\n" ;
    delete [] log ;
}

GLuint ShaderBase::initshaders (GLenum type, const char *filename)
{
    GLuint shader = glCreateShader(type) ;
    GLint compiled ;
    std::string str = textFileRead (filename) ;
    GLchar * cstr = new GLchar[str.size()+1] ;
    const GLchar * cstr2 = cstr ; // Weirdness to get a const char
    strcpy(cstr,str.c_str()) ;
    glShaderSource (shader, 1, &cstr2, NULL) ;
    glCompileShader (shader) ;
    glGetShaderiv (shader, GL_COMPILE_STATUS, &compiled) ;
    if (!compiled)
    {
        shadererrors (shader) ;
        throw 3 ;
    }
    delete [] cstr;
//    delete [] cstr2;
    return shader ;
}

GLuint ShaderBase::initprogram (GLuint vertexshader, GLuint fragmentshader)
{
    GLuint program = glCreateProgram() ;
    GLint linked ;
    glAttachShader(program, vertexshader) ;
    glAttachShader(program, fragmentshader) ;
    glLinkProgram(program) ;
    glGetProgramiv(program, GL_LINK_STATUS, &linked) ;
    if (linked) glUseProgram(program) ;
    else
    {
        programerrors(program) ;
        throw 4 ;
    }

    // When the program is created, the shaders are no longer needed
    glDetachShader(program, vertexshader);
    glDetachShader(program, fragmentshader);
    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);

    return program ;
}
