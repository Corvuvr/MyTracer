#include "Scene.h"
#include <iostream>
#include <fstream>
#include <sstream>


void Mesh::loadMesh(const char* filename)
{
	// Парсер obj-файлов

    std::ifstream in(filename, std::ios::in);
    if (!in)
    {
        std::cerr << "Cannot open " << filename << std::endl;
        exit(1);

    }
    std::string line;
    glm::vec3 vert;
    std::vector<glm::vec3> verts;
    std::vector<int> faceIndex;
    std::vector<int> textureIndex;

    glm::vec3 maxXYZ, minXYZ;
    maxXYZ.x = FLT_MIN;
    maxXYZ.y = FLT_MIN;
    maxXYZ.z = FLT_MIN;
    minXYZ.z = FLT_MAX;
    minXYZ.z = FLT_MAX;
    minXYZ.z = FLT_MAX;


    while (std::getline(in, line))
    {
        //check v for vertices
        if (line.substr(0, 2) == "v ") 
        {
            std::istringstream v(line.substr(2));
            
            double x, y, z;
            v >> vert.x; v >> vert.y; v >> vert.z;
            verts.push_back(vert);

            // getting minXYZ and maxXYZ

            if (vert.x > maxXYZ.x) maxXYZ.x = vert.x;
            if (vert.y > maxXYZ.y) maxXYZ.y = vert.y;
            if (vert.z > maxXYZ.z) maxXYZ.z = vert.z;

            if (vert.x < minXYZ.x) minXYZ.x = vert.x;
            if (vert.x < minXYZ.y) minXYZ.y = vert.y;
            if (vert.x < minXYZ.z) minXYZ.z = vert.z;
        }

        //check for faces
        else if (line.substr(0, 2) == "f ") {
            int a, b, c, d; //to store mesh index

            const char* chh = line.c_str();
            sscanf(chh, "f %i/%*i/%*i %i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &a, &b, &c, &d); 
            //here it read the line start with f and store the corresponding values in the variables
            //std::cout << chh << "\n";
            a--; b--; c--; d--;
            //std::cout << "Face: [ "<< a << " " << b << " " << c << " " << d << " ]\n";
            faceIndex.push_back(a); 
            faceIndex.push_back(b); 
            faceIndex.push_back(c);
            //faceIndex.push_back(d);
        }

    }
    //std::cout << "\n" << faceIndex.size() << "\n\n";
    //the mesh data is finally calculated here

#ifdef Quads
    for (unsigned int i = 0; i < faceIndex.size() - 3; i += 4)
    {
        //общая сторона - i0i2
        Triangle triangle;
        triangle.v0 = verts[faceIndex[i + 0]];
        triangle.v2 = verts[faceIndex[i + 2]];
        triangle.v1 = verts[faceIndex[i + 1]];

        this->Triangles.push_back(triangle);

        triangle.v0 = verts[faceIndex[i + 0]];
        triangle.v1 = verts[faceIndex[i + 2]];
        triangle.v2 = verts[faceIndex[i + 3]];

        this->Triangles.push_back(triangle);
    }
#endif // Quads

    for (unsigned int i = 0; i < faceIndex.size() - 2; i += 3)
    {
        //общая сторона - i0i2
        Triangle triangle;
        triangle.v0 = verts[faceIndex[i + 0]];
        triangle.v2 = verts[faceIndex[i + 2]];
        triangle.v1 = verts[faceIndex[i + 1]];

        this->Triangles.push_back(triangle);

    }

    this->boundingBox = BoundingBox(maxXYZ, minXYZ);
    std::cout << maxXYZ.x << " " << maxXYZ.y << " " << maxXYZ.z << "\n";
    std::cout << minXYZ.x << " " << minXYZ.y << " " << minXYZ.z << "\n";
}
