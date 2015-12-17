#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "TerrainMesh.h"
#include "Vec3.h"


/*
* namespace contenant les methodes d'exports de Mesh (singleton)
*/
namespace MeshWriter
{
	/*
	* export d'un objet Mesh en format .obj
	* obj : objet Mesh a ecrire
	* filename : nom du fichier dans lequel va etre exporte le mesh
	*/
	void exportObj ( TerrainMesh& obj, std::string filename )
	{
		std::vector<Vec3<float>> points = obj.getPoints();
		std::vector<Vec3<unsigned int>> faces = obj.getFaces();
		std::vector<Vec3<float>> textures = obj.getTextures();
		std::vector<Vec3<float>> normales = obj.getFacesNormales();
		std::vector<Vec3<unsigned int>> facesTextures = obj.getFacesTextures();
		std::vector<Vec3<unsigned int>> facesNormales = obj.getFacesNormalesIndex ( );

		{
			std::ofstream file ( filename );

			for ( unsigned int i = 0; i < points.size ( ); i++ )
				file << "v " << points[i].x << " " << points[i].y << " " << points[i].z << std::endl;

			for ( unsigned int i = 0; i < textures.size ( ); i++ )
				file << "vt " << textures[i].x << " " << textures[i].y << " " << textures[i].z << std::endl;

			for ( unsigned int i = 0; i < normales.size ( ); i++ )
				file << "vn " << normales[i].x << " " << normales[i].y << " " << normales[i].z << std::endl;

			if ( facesTextures.size ( ) > 0 && facesNormales.size ( ) > 0 ) {
				for ( unsigned int i = 0; i < faces.size ( ); i++ ) {
					file << "f " << faces[i].x << "/" << facesTextures[i].x << "/" << facesNormales[i].x
						<< " " << faces[i].y << "/" << facesTextures[i].y << "/" << facesNormales[i].y
						<< " " << faces[i].z << "/" << facesTextures[i].z << "/" << facesNormales[i].z << std::endl;
				}
			}
			else if ( facesTextures.size ( ) > 0 ) {
				for ( unsigned int i = 0; i < faces.size ( ); i++ ) {
					file << "f " << faces[i].x << "/" << facesTextures[i].x
						<< " " << faces[i].y << "/" << facesTextures[i].y
						<< " " << faces[i].z << "/" << facesTextures[i].z << std::endl;
				}
			}
			else if ( facesNormales.size ( ) > 0 ) {
				for ( unsigned int i = 0; i < faces.size ( ); i++ ) {
					file << "f " << faces[i].x << "//" << facesNormales[i].x
						<< " " << faces[i].y << "//" << facesNormales[i].y
						<< " " << faces[i].z << "//" << facesNormales[i].z << std::endl;
				}
			}
			else {
				for ( unsigned int i = 0; i < faces.size ( ); i++ ) {
					file << "f " << faces[i].x
						<< " " << faces[i].y
						<< " " << faces[i].z << std::endl;
				}
			}

			file.close ( );
		}		

		return;
	}


	void exportData ( TerrainMesh& obj, std::string &data ) 
	{
		std::vector<Vec3<float>> points = obj.getPoints ( );
		std::vector<Vec3<unsigned int>> faces = obj.getFaces ( );
		std::vector<Vec3<float>> textures = obj.getTextures ( );
		std::vector<Vec3<float>> normales = obj.getFacesNormales ( );
		std::vector<Vec3<unsigned int>> facesTextures = obj.getFacesTextures ( );
		std::vector<Vec3<unsigned int>> facesNormales = obj.getFacesNormalesIndex ( );

		{
			std::ostringstream stream;

			stream << std::fixed;

			for ( unsigned int i = 0; i < points.size ( ); i++ )
				stream << "v " << points[i].x << " " << points[i].y << " " << points[i].z << std::endl;

			for ( unsigned int i = 0; i < textures.size ( ); i++ )
				stream << "vt " << textures[i].x << " " << textures[i].y << " " << textures[i].z << std::endl;

			for ( unsigned int i = 0; i < normales.size ( ); i++ )
				stream << "vn " << normales[i].x << " " << normales[i].y << " " << normales[i].z << std::endl;

			if ( facesTextures.size ( ) > 0 && facesNormales.size ( ) > 0 ) {
				for ( unsigned int i = 0; i < faces.size ( ); i++ ) {
					stream
						<< "f " << faces[i].x << "/" << facesTextures[i].x << "/" << facesNormales[i].x
						<< " " << faces[i].y << "/" << facesTextures[i].y << "/" << facesNormales[i].y
						<< " " << faces[i].z << "/" << facesTextures[i].z << "/" << facesNormales[i].z << std::endl;
				}
			}
			else if ( facesTextures.size ( ) > 0 ) {
				for ( unsigned int i = 0; i < faces.size ( ); i++ ) {
					stream
						<< "f " << faces[i].x << "/" << facesTextures[i].x
						<< " " << faces[i].y << "/" << facesTextures[i].y
						<< " " << faces[i].z << "/" << facesTextures[i].z << std::endl;
				}
			}
			else if ( facesNormales.size ( ) > 0 ) {
				for ( unsigned int i = 0; i < faces.size ( ); i++ ) {
					stream
						<< "f " << faces[i].x << "//" << facesNormales[i].x
						<< " " << faces[i].y << "//" << facesNormales[i].y
						<< " " << faces[i].z << "//" << facesNormales[i].z << std::endl;
				}
			}
			else {
				for ( unsigned int i = 0; i < faces.size ( ); i++ ) {
					stream
						<< "f " << faces[i].x
						<< " " << faces[i].y
						<< " " << faces[i].z << std::endl;
				}
			}

			data = stream.str ( );
		}

		return;
	}
};
