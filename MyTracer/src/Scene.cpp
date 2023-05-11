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
    cl_float3 vert;
    std::vector<cl_float3> verts;
    std::vector<int> faceIndex;
    std::vector<int> textureIndex;

    cl_float3 maxXYZ, minXYZ;
    maxXYZ.s[0] = FLT_MIN;
    maxXYZ.s[1] = FLT_MIN;
    maxXYZ.s[2] = FLT_MIN;
    minXYZ.s[0] = FLT_MAX;
    minXYZ.s[1] = FLT_MAX;
    minXYZ.s[2] = FLT_MAX;


    while (std::getline(in, line))
    {
        //check v for vertices
        if (line.substr(0, 2) == "v ") 
        {
            std::istringstream v(line.substr(2));
            
            double x, y, z;
            v >> vert.s[0]; v >> vert.s[1]; v >> vert.s[2];
            verts.push_back(vert);

            // getting minXYZ and maxXYZ

            if (vert.s[0] > maxXYZ.s[0]) maxXYZ.s[0] = vert.s[0];
            if (vert.s[1] > maxXYZ.s[1]) maxXYZ.s[1] = vert.s[1];
            if (vert.s[2] > maxXYZ.s[2]) maxXYZ.s[2] = vert.s[2];

            if (vert.s[0] < minXYZ.s[0]) minXYZ.s[0] = vert.s[0];
            if (vert.s[1] < minXYZ.s[1]) minXYZ.s[1] = vert.s[1];
            if (vert.s[2] < minXYZ.s[2]) minXYZ.s[2] = vert.s[2];
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
        struct Triangle triangle;
        triangle.v0 = verts[faceIndex[i + 0]];
        triangle.v2 = verts[faceIndex[i + 2]];
        triangle.v1 = verts[faceIndex[i + 1]];

        this->Triangles.push_back(triangle);

    }

    //this->boundingBox = BoundingBox(maxXYZ, minXYZ);
    //std::cout << maxXYZ.x << " " << maxXYZ.y << " " << maxXYZ.z << "\n";
    //std::cout << minXYZ.x << " " << minXYZ.y << " " << minXYZ.z << "\n";
}
