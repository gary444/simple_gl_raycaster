#ifndef GL_STRUCTS_HPP
#define GL_STRUCTS_HPP

#include <vector>
#include <iostream>

#include <glm/glm.hpp>

class Quad
{
        // screen quad setup
    float vertices[12] = {
         0.999f,  0.999f, 0.5f,  // top right
         0.999f,  0.001f, 0.5f, // bottom right
         0.001f,  0.001f, 0.5f, // bottom left
         0.001f,  0.999f, 0.5f  // top left 
    };

    // float texCoords[8] = {
    //      1.0f,  1.0f,  // top right
    //      1.0f,  0.0f,  // bottom right
    //      0.0f,  0.0f,  // bottom left
    //      0.0f,  1.0f   // top left 
    // };
    unsigned int indices[6] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };  

    GLuint quadVAO = 0; 
    GLuint quadVBO = 0; 
    // GLuint quadTBO = 0;
    GLuint quadEBO = 0;

public:

    Quad() {
        
        glGenVertexArrays(1,&quadVAO);
        glGenBuffers(1,&quadVBO);
        // glGenBuffers(1,&quadTBO);
        glGenBuffers(1,&quadEBO);

        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


        // 4. then set the vertex attributes pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);  

        // glBindBuffer(GL_ARRAY_BUFFER, quadTBO);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

        // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        // glEnableVertexAttribArray(1);  

        //  3. copy our index array in a element buffer for OpenGL to use
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    }


    void render() const {

        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        // glBindBuffer(GL_ARRAY_BUFFER, quadTBO);
        glBindBuffer(GL_ARRAY_BUFFER, quadEBO);


        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }

};





class PrimitiveSetBufferObject {

public:
	PrimitiveSetBufferObject() {

        num_vertices = 0;

        glGenVertexArrays( 1, &VAO );
        glBindVertexArray(VAO);

        glGenBuffers( 1, &VBO );
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

	}

	virtual ~PrimitiveSetBufferObject(){

        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
	}

	virtual void render() const {

        // std::cout << "PrimitiveSetBufferObject::render()" << std::endl;
        

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // std::cout << "PrimitiveSetBufferObject::render() done" << std::endl;

	}

    void update_vertices(std::vector<glm::vec3>& vertices) {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size()*3, vertices.data(),GL_STATIC_DRAW);
        num_vertices = vertices.size();
    }
    void update_vertices(std::vector<float>& vertices) {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), vertices.data(),GL_STATIC_DRAW);
        num_vertices = vertices.size() / 3;
    }

protected:
	GLuint VAO = 0;
    GLuint VBO = 0;

    uint32_t num_vertices;

};

class AxesMarkers : public PrimitiveSetBufferObject
{

public:
    AxesMarkers() : PrimitiveSetBufferObject(){

        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*verts.size(), verts.data(),GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0 );

        glGenBuffers( 1, &VCO );
        glBindBuffer(GL_ARRAY_BUFFER, VCO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*colours.size(), colours.data(),GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0 );

    }

	virtual ~AxesMarkers() {
        glDeleteBuffers(1, &VCO);
	}

    void render() const {
    	PrimitiveSetBufferObject::render();
        glBindBuffer(GL_ARRAY_BUFFER, VCO);
        glDrawArrays(GL_LINES, 0, 6);
    }

private:

    std::vector<float> verts = {0.f, 0.f, 0.f,
                                0.1f, 0.f, 0.f,
                                0.f, 0.f, 0.f,
                                0.f, 0.1f, 0.f,
                                0.f, 0.f, 0.f,
                                0.f, 0.f, 0.1f};


    std::vector<float> colours = {1.f, 0.f, 0.f,
                                  1.f, 0.f, 0.f,
                                  0.f, 1.f, 0.f,
                                  0.f, 1.f, 0.f,
                                  0.f, 0.f, 1.f,
                                  0.f, 0.f, 1.f};

    GLuint VCO;

};




class LineSet : public PrimitiveSetBufferObject
{
public:
    LineSet() {}
    LineSet(std::vector<glm::vec3>& vertices) : PrimitiveSetBufferObject() {

        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size() * 3, vertices.data(),GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0 );
        glEnableVertexAttribArray(0);
        num_vertices = vertices.size();
    }

    LineSet(std::vector<float>& vertices) : PrimitiveSetBufferObject() {

        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), vertices.data(),GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0 );
        glEnableVertexAttribArray(0);
        num_vertices = vertices.size() / 3;
    }

    void render() const{
    	PrimitiveSetBufferObject::render();
        glDrawArrays(GL_LINES, 0, num_vertices);
    }



};

class BoundingBoxes : public LineSet
{

public:
    // BoundingBoxes() {}

    void upload(){
        glGenVertexArrays( 1, &VAO );
        glBindVertexArray(VAO);
        glGenBuffers( 1, &VBO );
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*all_verts.size() * 3, all_verts.data(),GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0 );
        glEnableVertexAttribArray(0);
        num_vertices = all_verts.size();
    }

    void add_bb (glm::vec3 min, glm::vec3 max) {

        const glm::vec3 size = max - min;

        for (auto v : box_verts){
            all_verts.push_back( min + v*size );
        }
        // num_vertices += (box_verts.size());
    }

private:

    std::vector<glm::vec3> all_verts;
    std::vector<glm::vec3> box_verts = {
                                glm::vec3(0.f, 0.f, 0.f),  glm::vec3(0.f, 1.f, 0.f),
                                glm::vec3(0.f, 1.f, 0.f),  glm::vec3(1.f, 1.f, 0.f),
                                glm::vec3(1.f, 1.f, 0.f),  glm::vec3(1.f, 0.f, 0.f),
                                glm::vec3(1.f, 0.f, 0.f),  glm::vec3(0.f, 0.f, 0.f),

                                glm::vec3(0.f, 0.f, 1.f),  glm::vec3(0.f, 1.f, 1.f),
                                glm::vec3(0.f, 1.f, 1.f),  glm::vec3(1.f, 1.f, 1.f),
                                glm::vec3(1.f, 1.f, 1.f),  glm::vec3(1.f, 0.f, 1.f),
                                glm::vec3(1.f, 0.f, 1.f),  glm::vec3(0.f, 0.f, 1.f),
                                
                                glm::vec3(0.f, 0.f, 0.f),  glm::vec3(0.f, 0.f, 1.f),
                                glm::vec3(0.f, 1.f, 0.f),  glm::vec3(0.f, 1.f, 1.f),
                                glm::vec3(1.f, 1.f, 0.f),  glm::vec3(1.f, 1.f, 1.f),
                                glm::vec3(1.f, 0.f, 0.f),  glm::vec3(1.f, 0.f, 1.f)
                            };
                                
};

// class ColouredBoundingBoxes : public BoundingBoxes
// {
    
// };



class PointSet : public PrimitiveSetBufferObject
{
public:
    PointSet(std::vector<float>& vertices) : PrimitiveSetBufferObject() {

	    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), vertices.data(),GL_STATIC_DRAW);
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0 );
	    glEnableVertexAttribArray(0);

        num_vertices = vertices.size() / 3;

    }
    PointSet(std::vector<glm::vec3>& vertices) : PrimitiveSetBufferObject() {

	    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size()*3, vertices.data(),GL_STATIC_DRAW);
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0 );
	    glEnableVertexAttribArray(0);

        num_vertices = vertices.size();

    }
    void render() const{
    	PrimitiveSetBufferObject::render();
        glDrawArrays(GL_POINTS, 0, num_vertices);
    }

    // void update_points(std::vector<glm::vec3>& vertices) {
    //     glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //     glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size()*3, vertices.data(),GL_STATIC_DRAW);
    //     num_vertices = vertices.size();
    // }

};


class ColouredPointSet : public PointSet
{
public:
    ColouredPointSet(std::vector<glm::vec3>& vertices) : PointSet(vertices) {

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*) 0 );
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*) 12 );

		num_vertices = vertices.size() / 2;
    }


};


class MeshWithNormals : public PrimitiveSetBufferObject
{
public:

    MeshWithNormals() : PrimitiveSetBufferObject() {
        glGenBuffers( 1, &NBO );
    }

    MeshWithNormals(std::vector<float>& vertices, std::vector<float>& normals) : PrimitiveSetBufferObject() {

        glGenBuffers( 1, &NBO );
   
        update_vertices_and_normals(vertices, normals);
    }

    void update_vertices_and_normals(std::vector<float>& vertices, std::vector<float>& normals) {

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), vertices.data(),GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0 );

        // // Normal
        glBindBuffer(GL_ARRAY_BUFFER, NBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*normals.size(), normals.data(),GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, ( GLvoid * ) 0 );

        num_vertices = vertices.size() / 3;

        glBindVertexArray(0);

    }


    virtual ~MeshWithNormals() {
        glBindVertexArray(VAO);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDeleteBuffers(1, &NBO);
    }

    void render() const{
    	PrimitiveSetBufferObject::render();

		glBindBuffer(GL_ARRAY_BUFFER, NBO);
        glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    }            
private:

    GLuint NBO = 0;
};

class MeshWithNormalsAndScalar : public MeshWithNormals
{
public:
    MeshWithNormalsAndScalar(std::vector<float>& vertices, 
                             std::vector<float>& normals,
                             std::vector<float>& scalar
                             ) : MeshWithNormals(vertices, normals) { 

        glBindVertexArray(VAO);

        // indices
        glGenBuffers(1, &SBO);
        glBindBuffer(GL_ARRAY_BUFFER, SBO);
        glBufferData(GL_ARRAY_BUFFER, scalar.size() * sizeof(float), scalar.data(), GL_STATIC_DRAW);


        glEnableVertexAttribArray(2);
        glVertexAttribPointer( 2, 1, GL_FLOAT, GL_FALSE, 0, ( GLvoid * ) 0 );

        glBindVertexArray(0);


    }

    ~MeshWithNormalsAndScalar(){
        glDeleteBuffers(1, &SBO);
    }
    void render() const{
        MeshWithNormals::render();
    }

private:
    GLuint SBO = 0;
};




class IndexedMeshWithNormals : public MeshWithNormals
{
public:

    IndexedMeshWithNormals() : MeshWithNormals() {
        glGenBuffers(1, &IBO);
    }

    IndexedMeshWithNormals(std::vector<float>& vertices, std::vector<uint32_t>& indices, std::vector<float>& normals) : 
        MeshWithNormals(vertices, normals) {

        // indices
        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

        num_indices = indices.size();


    }

    void update_vertices_indices_and_normals( std::vector<float>& vertices, std::vector<uint32_t>& indices, std::vector<float>& normals)  {

        MeshWithNormals::update_vertices_and_normals(vertices, normals);

        // indices
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
        glBindVertexArray(0);

        num_indices = indices.size();
   
    }

    virtual ~IndexedMeshWithNormals() {
        glDeleteBuffers(1, &IBO);
    }

    void render() const{
        PrimitiveSetBufferObject::render();
        glBindBuffer(GL_ARRAY_BUFFER, NBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, (void*) 0);

    }            


private:
    uint32_t num_indices = 0;
    GLuint NBO = 0;
    GLuint IBO = 0;
};

class IndexedMeshWithNormalsAndScalar : public IndexedMeshWithNormals
{
public:
    IndexedMeshWithNormalsAndScalar(std::vector<float>& vertices, 
                                    std::vector<uint32_t>& indices, 
                                    std::vector<float>& normals,
                                    std::vector<float>& scalar
                                    ) : IndexedMeshWithNormals(vertices, indices, normals)
                                    { 

        glBindVertexArray(VAO);

        // indices
        glGenBuffers(1, &SBO);
        glBindBuffer(GL_ARRAY_BUFFER, SBO);
        glBufferData(GL_ARRAY_BUFFER, scalar.size() * sizeof(float), scalar.data(), GL_STATIC_DRAW);


        glEnableVertexAttribArray(2);
        glVertexAttribPointer( 2, 1, GL_FLOAT, GL_FALSE, 0, ( GLvoid * ) 0 );

        glBindVertexArray(0);


    }

    ~IndexedMeshWithNormalsAndScalar(){
        glDeleteBuffers(1, &SBO);
    }
    void render() const{
        IndexedMeshWithNormals::render();
    }

private:
    GLuint SBO = 0;
};


class EmptyVAOForDrawingArray
{
public:
    static void bind() {
        static EmptyVAOForDrawingArray instance{};
        glBindVertexArray(instance.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, instance.VBO);
        glEnable(GL_RASTERIZER_DISCARD);
    }
    static void unbind(){
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisable(GL_RASTERIZER_DISCARD);
    }
private:

    EmptyVAOForDrawingArray() {
        std::cout << "Create VAO" << std::endl;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    }
    ~EmptyVAOForDrawingArray(){

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
    EmptyVAOForDrawingArray(EmptyVAOForDrawingArray const&) = delete;
    EmptyVAOForDrawingArray& operator=(EmptyVAOForDrawingArray const&) = delete;

    GLuint VAO, VBO;

};



#endif