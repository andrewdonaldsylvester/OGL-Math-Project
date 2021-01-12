#include "shader_loading.hpp"


std::string loadFile(const char* path)
{
    std::string content;
    std::ifstream file_stream;
    
    file_stream.open(path, std::ios::in);
    
    if(!file_stream.is_open())
    {
        std::cerr << "Could not read file " << path << ". File does not exist." << std::endl;
        return NULL;
    }

    
    std::string line = "";
    
    while(!file_stream.eof()) {
        std::getline(file_stream, line);
        content.append(line + "\n");
    }

    file_stream.close();
    std::cout << "'" << content << "'" << std::endl;
    
    return content;
}

GLuint loadShaders(const char* vertex_path, const char* fragment_path)
{
    std::string vertex_src_str   = loadFile(vertex_path);
    std::string fragment_src_str = loadFile(fragment_path);
    
    GLchar* vertex_src   = (GLchar*) vertex_src_str.c_str();
    GLchar* fragment_src = (GLchar*) fragment_src_str.c_str();
    
    int success;
    char infoLog[512];
    
    GLuint vertex_shader, fragment_shader, shader_program;
    
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_src, NULL);
    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
    
    
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_src, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
    
    
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
      
    
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    return shader_program;
}

