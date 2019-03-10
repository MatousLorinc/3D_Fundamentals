/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Game.cpp																			  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "Mat3.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	model("resources\\cube.obj"),
	sbTex(Surface::FromFile(L"resources\\cube_albedo_tex.png"))
{
}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float dt = 1.0f / 60.0f;
	if( wnd.kbd.KeyIsPressed( 'Q' ) )
	{
		theta_x = wrap_angle( theta_x + dTheta * dt );
	}
	if( wnd.kbd.KeyIsPressed( 'W' ) )
	{
		theta_y = wrap_angle( theta_y + dTheta * dt );
	}
	if( wnd.kbd.KeyIsPressed( 'E' ) )
	{
		theta_z = wrap_angle( theta_z + dTheta * dt );
	}
	if( wnd.kbd.KeyIsPressed( 'A' ) )
	{
		theta_x = wrap_angle( theta_x - dTheta * dt );
	}
	if( wnd.kbd.KeyIsPressed( 'S' ) )
	{
		theta_y = wrap_angle( theta_y - dTheta * dt );
	}
	if( wnd.kbd.KeyIsPressed( 'D' ) )
	{
		theta_z = wrap_angle( theta_z - dTheta * dt );
	}
	if( wnd.kbd.KeyIsPressed( 'R' ) )
	{
		offset_z += 2.0f * dt;
	}
	if( wnd.kbd.KeyIsPressed( 'F' ) )
	{
		offset_z -= 2.0f * dt;
	}
}

void Game::ComposeFrame()
{
	const Color colors[12] = {
		Colors::White,
		Colors::Blue,
		Colors::Cyan,
		Colors::Gray,
		Colors::Green,
		Colors::Magenta,
		Colors::LightGray,
		Colors::Red,
		Colors::Yellow,
		Colors::White,
		Colors::Blue,
		Colors::Cyan
	};

	// for white background
	//for (int x = 0; x < Graphics::ScreenWidth; x++)
	//{
	//	for (int y = 0; y < Graphics::ScreenHeight; y++)
	//	{
	//		gfx.PutPixel(x, y, Colors::White);
	//	}
	//}

	// copy vertices and normals from model
	std::vector<Vec3> vertices = model.vertices;
	std::vector<Vec3> normals = model.normals;
	std::vector<Vec2> textureVertices = model.textureVertices;
	Vec3 offSet = { 0.0f,0.0f,offset_z + 2.f };
	// generate rotation matrix from euler angles
	const Mat3 rot =
		Mat3::RotationX(theta_x) *
		Mat3::RotationY(theta_y) *
		Mat3::RotationZ(theta_z);
	// transform vertices from model space -> world (/view) space
	for (auto& v : vertices)
	{
		v *= rot;
		v += offSet;
	}
	// transform normals from model space -> world (/view) space
	for (auto& vn : normals)
	{
		vn *= rot;
	}
	// backface culling test (must be done in world (/view) space)
	for (int i = 0; i < model.triangles.size(); i++)
	{
		const Vec3& v0 = vertices[model.triangles[i].v0];
		const Vec3& v1 = vertices[model.triangles[i].v1];
		const Vec3& v2 = vertices[model.triangles[i].v2];
		const Vec3& vn = normals[model.triangles[i].vn];
		model.triangles[i].cullFlag = vn * v0 > 0.0f;
	}
	for (auto& v : vertices)
	{
		pst.Transform(v);
	}

	for (int i = 0; i < model.triangles.size(); i++)
	{
		const Vec3& v0 = vertices[model.triangles[i].v0];
		const Vec3& v1 = vertices[model.triangles[i].v1];
		const Vec3& v2 = vertices[model.triangles[i].v2];
		const Vec2& vt0 = textureVertices[model.triangles[i].vt0];
		const Vec2& vt1 = textureVertices[model.triangles[i].vt1];
		const Vec2& vt2 = textureVertices[model.triangles[i].vt2];

		// skip triangles previously determined to be back-facing
		if(!model.triangles[i].cullFlag)
		{
			//gfx.DrawTriangle(v0,v1,v2,colors[i] );
			TexVertex tv0 = TexVertex(v0, vt0);
			TexVertex tv1 = TexVertex(v1, vt1);
			TexVertex tv2 = TexVertex(v2, vt2);
			gfx.DrawTriangleTex(tv0, tv1, tv2, sbTex);
		}
	}
}