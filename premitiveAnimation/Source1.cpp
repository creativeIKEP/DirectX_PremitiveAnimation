//-----------------------------------------------------------------------------
// File: Lights.cpp
//
// Desc: Rendering 3D geometry is much more interesting when dynamic lighting
//       is added to the scene. To use lighting in D3D, you must create one or
//       lights, setup a material, and make sure your geometry contains surface
//       normals. Lights may have a position, a color, and be of a certain type
//       such as directional (light comes from one direction), point (light
//       comes from a specific x,y,z coordinate and radiates in all directions)
//       or spotlight. Materials describe the surface of your geometry,
//       specifically, how it gets lit (diffuse color, ambient color, etc.).
//       Surface normals are part of a vertex, and are needed for the D3D's
//       internal lighting calculations.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------


#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )




//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; // Buffer to hold vertices
LPDIRECT3DVERTEXBUFFER9 g_pVB2 = NULL; // Buffer to hold vertices



									  // A structure for our custom vertex type. We added a normal, and omitted the
									  // color (which is provided by the material)
struct CUSTOMVERTEX
{
	D3DXVECTOR3 position; // The 3D position for the vertex
	//D3DXVECTOR3 normal;   // The surface normal for the vertex
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL)

float g_aspect = 1.6f;


//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D(HWND hWnd)
{
	// Create the D3D object.
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	// Set up the structure used to create the D3DDevice. Since we are now
	// using more complex geometry, we will create a device with a zbuffer.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// Create the D3DDevice
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice)))
	{
		return E_FAIL;
	}
	

	// Turn off culling
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// Turn on the zbuffer
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);


	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InitGeometry()
// Desc: Creates the scene geometry
//-----------------------------------------------------------------------------
HRESULT InitGeometry()
{
	// Create the vertex buffer.
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(50 * 2 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}

	// Fill the vertex buffer. We are algorithmically generating a cylinder
	// here, including the normals, which are used for lighting.
	CUSTOMVERTEX* pVertices;
	if (FAILED(g_pVB->Lock(0, 0, (void**)&pVertices, 0)))
		return E_FAIL;
	
	float len = 0.1f;
	pVertices[0].position = D3DXVECTOR3(-len, -len, -len);
	pVertices[1].position = D3DXVECTOR3(-len, len, -len);
	pVertices[2].position = D3DXVECTOR3(len, -len, -len);
	pVertices[3].position = D3DXVECTOR3(len, len, -len);
	pVertices[4].position = D3DXVECTOR3(len, -len, len);
	pVertices[5].position = D3DXVECTOR3(len, len, len);
	pVertices[6].position = D3DXVECTOR3(-len, -len, len);
	pVertices[7].position = D3DXVECTOR3(-len, len, len);
	pVertices[8].position = D3DXVECTOR3(-len, -len, -len);
	pVertices[9].position = D3DXVECTOR3(-len, len, -len);
	pVertices[10].position = D3DXVECTOR3(len, len, -len);
	pVertices[11].position = D3DXVECTOR3(len, len, len);
	pVertices[12].position = D3DXVECTOR3(-len, len, len);
	pVertices[13].position = D3DXVECTOR3(-len, len, -len);
	pVertices[14].position = D3DXVECTOR3(-len, -len, -len);
	pVertices[15].position = D3DXVECTOR3(len, -len, -len);
	pVertices[16].position = D3DXVECTOR3(len, -len, len);
	pVertices[17].position = D3DXVECTOR3(-len, -len, len);
	pVertices[18].position = D3DXVECTOR3(-len, -len, -len);

	pVertices[19].position = D3DXVECTOR3(-len, -len, 0);
	pVertices[20].position = D3DXVECTOR3(-len, len, 0);
	pVertices[21].position = D3DXVECTOR3(len, len, 0);
	pVertices[22].position = D3DXVECTOR3(len, -len, 0);
	pVertices[23].position = D3DXVECTOR3(-len, -len, 0);
	g_pVB->Unlock();




	// Create the vertex buffer.
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(50 * 2 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB2, NULL)))
	{
		return E_FAIL;
	}

	// Fill the vertex buffer. We are algorithmically generating a cylinder
	// here, including the normals, which are used for lighting.
	CUSTOMVERTEX* pVertices2;
	if (FAILED(g_pVB2->Lock(0, 0, (void**)&pVertices2, 0)))
		return E_FAIL;

	pVertices2[0].position = D3DXVECTOR3(-len, -len, 0);
	pVertices2[1].position = D3DXVECTOR3(-len, len, 0);
	pVertices2[2].position = D3DXVECTOR3(len, len, 0);
	pVertices2[3].position = D3DXVECTOR3(len, -len, 0);
	pVertices2[4].position = D3DXVECTOR3(-len, -len, 0);
	
	g_pVB2->Unlock();

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{
	if (g_pVB != NULL)
		g_pVB->Release();

	if (g_pVB2 != NULL)
		g_pVB2->Release();

	if (g_pd3dDevice != NULL)
		g_pd3dDevice->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();
}



//-----------------------------------------------------------------------------
// Name: SetupMatrices()
// Desc: Sets up the world, view, and projection transform matrices.
//-----------------------------------------------------------------------------
VOID SetupMatrices()
{
	

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.

	//view行列の作成
	//カメラの座標
	D3DXVECTOR3 vEyePt(0.0f, -5.0f, -5.0f);
	//注視点
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	//カメラの上
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;

	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
	

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	//射影変換行列の生成
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, g_aspect, 1.0f, 100.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	
}



//-----------------------------------------------------------------------------
// Name: SetupLights()
// Desc: Sets up the lights and materials for the scene.
//-----------------------------------------------------------------------------
VOID SetupLights()
{
	// Set up a material. The material here just has the diffuse and ambient
	// colors set to yellow. Note that only one material can be used at a time.
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = mtrl.Emissive.r=0.3f;
	mtrl.Diffuse.g = mtrl.Ambient.g = mtrl.Emissive.g = 0.1f;
	mtrl.Diffuse.b = mtrl.Ambient.b = mtrl.Emissive.b= 0.5f;
	mtrl.Diffuse.a = mtrl.Ambient.a = mtrl.Emissive.a = 1.0f;
	g_pd3dDevice->SetMaterial(&mtrl);
	

	// Set up a white, directional light, with an oscillating direction.
	// Note that many lights may be active at a time (but each one slows down
	// the rendering of our scene). However, here we are just using one. Also,
	// we need to set the D3DRS_LIGHTING renderstate to enable lighting
	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	vecDir = D3DXVECTOR3(0, -0.5f, 1.0f);
	//vecDirを正規化してlight.Directionにセット
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
	light.Range = 1000.0f;
	//lightを0番でセット
	g_pd3dDevice->SetLight(0, &light);
	//0番のライトを有効化
	g_pd3dDevice->LightEnable(0, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Finally, turn on some ambient light.
	g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);

}




//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------


float I1 = -3;
float I2 = 1.5;
float I3 = -3;
float K1 = 1.5;
float k2X = -1.3;
float k3X = -1.1;
float k2Y = 0;
float k3Y = 0.2;
float e1 = 1.5;
float e234 = 0.5;
float p1 = 1.5;
float p2 = D3DX_PI/2;

float i1_r = 0;
float i2_r = 0;
float i3_r = 0;
float k1_r = 0;
float k2_r = 0;
float k3_r = 0;
float e1_r = 0;
float e2_r = 0;
float e3_r = 0;
float e4_r = 0;
float p1_r = 0;
float p2_r = 0;

float pretime;
int indexMax = 1000;
float squarePos[1000][3];
int index = 0;
bool compFlag[12];
float endTime;
bool endGet = false;


VOID Render()
{
	// Clear the backbuffer and the zbuffer
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);


	// Begin the scene
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		
		// Setup the lights and materials
		SetupLights();

		// Setup the world, view, and projection matrices
		SetupMatrices();

		D3DXMATRIXA16 matWorld, moveMat, rotateMat;


		g_pd3dDevice->SetStreamSource(0, g_pVB2, 0, sizeof(CUSTOMVERTEX));
		//最初の位置の四角形を描画
		D3DXMatrixTranslation(&moveMat, -3, 1.5, 0.0);
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &moveMat);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);

		/*
		D3DXMatrixTranslation(&moveMat, -2.5, 1.5, 0.0);
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &moveMat);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);
		*/
		D3DXMatrixTranslation(&moveMat, -3, -1.5, 0.0);
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &moveMat);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);

		D3DXMatrixTranslation(&moveMat, -1.3, 1.5, 0.0);
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &moveMat);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);

		/*
		D3DXMatrixTranslation(&moveMat, -1.3, 0, 0.0);
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &moveMat);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);
		*/

		D3DXMatrixTranslation(&moveMat, 0.5, 1.5, 0.0);
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &moveMat);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);

		/*
		D3DXMatrixTranslation(&moveMat, 0.5, 0, 0.0);
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &moveMat);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);

		D3DXMatrixTranslation(&moveMat, 0.5, -1.5, 0.0);
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &moveMat);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);
		*/
		D3DXMatrixTranslation(&moveMat, 2.3, 1.5, 0.0);
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &moveMat);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);

		for (int i = 0; i < index; i++) {
			D3DXMatrixRotationZ(&rotateMat, squarePos[i][2]);
			D3DXMatrixTranslation(&moveMat, squarePos[i][0], squarePos[i][1], 0.0);
			matWorld = rotateMat * moveMat;
			g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);
		}



		// Render the vertex buffer contents
		g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);



		if (I1 <= -1.9) {
			D3DXMatrixRotationY(&rotateMat, -(timeGetTime() / 250.0));
			D3DXMatrixTranslation(&moveMat, I1, 1.5, 0.0);
			I1 += 0.5*(float)(timeGetTime() - pretime) / 1000.0;
			matWorld = rotateMat * moveMat;
			g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 18);

			if (abs((int)i1_r +(int)(timeGetTime() / 250.0)) >= D3DX_PI / 2 && index<indexMax) {
				squarePos[index][0] = I1;
				squarePos[index][1] = 1.5;
				squarePos[index][2] = 0;
				index++;
				i1_r = -(float)(timeGetTime() / 250.0);
			}
		}
		else if (!compFlag[0]) { compFlag[0] = true; }

		if (I2 >= -1.5) {
			D3DXMatrixRotationX(&rotateMat, -(timeGetTime() / 250.0));
			D3DXMatrixTranslation(&moveMat, -2.5, I2, 0.0);
			I2 -= 0.5*(float)(timeGetTime() - pretime) / 1000.0;
			matWorld = rotateMat * moveMat;
			g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 18);

			if (abs((int)i2_r + (int)(timeGetTime() / 250.0)) >= D3DX_PI / 2 && index<indexMax) {
				squarePos[index][0] = -2.5;
				squarePos[index][1] = I2;
				squarePos[index][2] = 0;
				index++;
				i2_r = -(float)(timeGetTime() / 250.0);
			}
		}
		else if (!compFlag[1]) { compFlag[1] = true; }

		if (I3 <= -1.9) {
			D3DXMatrixRotationY(&rotateMat, -(timeGetTime() / 250.0));
			D3DXMatrixTranslation(&moveMat, I3, -1.5, 0.0);
			I3 += 0.5*(float)(timeGetTime() - pretime) / 1000.0;
			matWorld = rotateMat * moveMat;
			g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 18);

			if (abs((int)i3_r + (int)(timeGetTime() / 250.0)) >= D3DX_PI / 2 && index<indexMax) {
				squarePos[index][0] = I3;
				squarePos[index][1] = -1.5;
				squarePos[index][2] = 0;
				index++;
				i3_r = -(float)(timeGetTime() / 250.0);
			}
		}
		else if (!compFlag[2]) { compFlag[2] = true; }

		if (K1 >= -1.7) {
			D3DXMatrixRotationX(&rotateMat, -(timeGetTime() / 250.0));
			D3DXMatrixTranslation(&moveMat, -1.3, K1, 0.0);
			K1 -= 0.5*(float)(timeGetTime() - pretime) / 1000.0;
			matWorld = rotateMat * moveMat;
			g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 18);

			if (abs((int)k1_r + (int)(timeGetTime() / 250.0)) >= D3DX_PI / 2 && index<indexMax) {
				squarePos[index][0] = -1.3;
				squarePos[index][1] = K1;
				squarePos[index][2] = 0;
				index++;
				k1_r = -(float)(timeGetTime() / 250.0);
			}
		}
		else if (!compFlag[3]) { compFlag[3] = true; }

		D3DXMATRIXA16 rotateMat2;
		if (k2X <= 0) {
			D3DXMatrixRotationZ(&rotateMat2, D3DX_PI / 4);
			D3DXMatrixRotationY(&rotateMat, -(timeGetTime() / 250.0));
			D3DXMatrixTranslation(&moveMat, k2X, k2Y, 0.0);
			k2Y += 0.5*(float)(timeGetTime() - pretime) / (sqrtf(2)*1000.0);
			k2X += 0.5*(float)(timeGetTime() - pretime) / (sqrtf(2)*1000.0);
			matWorld = rotateMat * rotateMat2 * moveMat;
			g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 18);

			if (abs((int)k2_r + (int)(timeGetTime() / 250.0)) >= D3DX_PI / 2 && index<indexMax) {
				squarePos[index][0] = k2X;
				squarePos[index][1] = k2Y;
				squarePos[index][2] = D3DX_PI / 4;
				index++;
				k2_r = -(float)(timeGetTime() / 250.0);
			}
		}
		else if (!compFlag[4]) { compFlag[4] = true; }

		if (k3X<=0) {
			D3DXMatrixRotationZ(&rotateMat2, -D3DX_PI / 3);
			D3DXMatrixRotationY(&rotateMat, (timeGetTime() / 250.0));
			D3DXMatrixTranslation(&moveMat, k3X, k3Y, 0.0);
			k3Y -= sqrtf(3)*0.5*(float)(timeGetTime() - pretime) / 2000.0;
			k3X += 0.5*(float)(timeGetTime() - pretime) / 2000.0;
			matWorld = rotateMat * rotateMat2 * moveMat;
			g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 18);

			if (abs((int)k3_r - (int)(timeGetTime() / 250.0)) >= D3DX_PI / 2 && index<indexMax) {
				squarePos[index][0] = k3X;
				squarePos[index][1] = k3Y;
				squarePos[index][2] = -D3DX_PI / 3;
				index++;
				k3_r = (float)(timeGetTime() / 250.0);
			}
		}
		else if (!compFlag[5]) { compFlag[5] = true; }

		if (e1 >= -1.5) {
			D3DXMatrixRotationX(&rotateMat, -(timeGetTime() / 250.0));
			D3DXMatrixTranslation(&moveMat, 0.5, e1, 0.0);
			e1 -= 0.5*(float)(timeGetTime() - pretime) / 1000.0;
			matWorld = rotateMat * moveMat;
			g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 18);

			if (abs((int)e1_r + (int)(timeGetTime() / 250.0)) >= D3DX_PI / 2 && index<indexMax) {
				squarePos[index][0] = 0.5;
				squarePos[index][1] = e1;
				squarePos[index][2] = 0;
				index++;
				e1_r = -(float)(timeGetTime() / 250.0);
			}
		}
		else if (!compFlag[6]) { compFlag[6] = true; }

		if (e234 <= 1.8) {
			D3DXMatrixRotationY(&rotateMat, -(timeGetTime() / 250.0));
			D3DXMatrixTranslation(&moveMat, e234, 1.5, 0.0);
			matWorld = rotateMat * moveMat;
			g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 18);

			if (abs((int)e2_r + (int)(timeGetTime() / 250.0)) >= D3DX_PI / 2 && index<indexMax) {
				squarePos[index][0] = e234;
				squarePos[index][1] = 1.5;
				squarePos[index][2] = 0;
				index++;
				e2_r = -(float)(timeGetTime() / 250.0);
			}


			D3DXMatrixRotationY(&rotateMat, -(timeGetTime() / 250.0));
			D3DXMatrixTranslation(&moveMat, e234, 0, 0.0);
			matWorld = rotateMat * moveMat;
			g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 18);

			if (abs((int)e3_r + (int)(timeGetTime() / 250.0)) >= D3DX_PI / 2 && index<indexMax) {
				squarePos[index][0] = e234;
				squarePos[index][1] = 0;
				squarePos[index][2] = 0;
				index++;
				e3_r = -(float)(timeGetTime() / 250.0);
			}


			D3DXMatrixRotationY(&rotateMat, -(timeGetTime() / 250.0));
			D3DXMatrixTranslation(&moveMat, e234, -1.5, 0.0);
			e234 += 0.5*(float)(timeGetTime() - pretime) / 1000.0;
			matWorld = rotateMat * moveMat;
			g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 18);

			if (abs((int)e4_r + (int)(timeGetTime() / 250.0)) >= D3DX_PI / 2 && index<indexMax) {
				squarePos[index][0] = e234;
				squarePos[index][1] = -1.5;
				squarePos[index][2] = 0;
				index++;
				e4_r = -(float)(timeGetTime() / 250.0);
			}
		}
		else{ compFlag[7] = true; compFlag[8] = true; compFlag[9] = true;}

		if (p1 >= -1.7) {
			D3DXMatrixRotationX(&rotateMat, -(timeGetTime() / 250.0));
			D3DXMatrixTranslation(&moveMat, 2.3, p1, 0.0);
			p1 -= 0.5*(float)(timeGetTime() - pretime) / 1000.0;
			matWorld = rotateMat * moveMat;
			g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 18);

			if (abs((int)p1_r + (int)(timeGetTime() / 250.0)) >= D3DX_PI / 2 && index<indexMax) {
				squarePos[index][0] = 2.3;
				squarePos[index][1] = p1;
				squarePos[index][2] = 0;
				index++;
				p1_r = -(float)(timeGetTime() / 250.0);
			}
		}
		else if (!compFlag[10]) { compFlag[10] = true; }

		if (cos(p2)>=-0.01) {
			D3DXMatrixRotationZ(&rotateMat, p2);
			D3DXMatrixRotationX(&rotateMat2, -(timeGetTime() / 250.0));
			D3DXMatrixTranslation(&moveMat, 0.85*cos(p2) + 2.3, 0.85*sin(p2) + 0.75, 0.0);
			p2 -= 0.5*(float)(timeGetTime() - pretime) / 1000.0;
			matWorld = rotateMat2 * rotateMat * moveMat;
			g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 18);

			if (abs((int)p2_r + (int)(timeGetTime() / 250.0)) >= D3DX_PI / 2 && index<indexMax) {
				squarePos[index][0] = 0.85*cos(p2) + 2.3;
				squarePos[index][1] = 0.85*sin(p2) + 0.75;
				squarePos[index][2] = p2;
				index++;
				p2_r = -(float)(timeGetTime() / 250.0);
			}
		}
		else if (!compFlag[11]) { compFlag[11] = true; }

		pretime = timeGetTime();


		int count = 0;
		for (int i = 0; i < 12; i++) {
			if (!compFlag[i]) { break; }
			else { count++; }
		}
		if (count >= 12) {
			
			if (!endGet) { endTime = timeGetTime(); endGet = true; }
			
			if ((timeGetTime() - endTime) > 1000 * 5) {
				I1 = -3;
				I2 = 1.5;
				I3 = -3;
				K1 = 1.5;
				k2X = -1.3;
				k3X = -1.1;
				k2Y = 0;
				k3Y = 0.2;
				e1 = 1.5;
				e234 = 0.5;
				p1 = 1.5;
				p2 = D3DX_PI / 2;
				i1_r = 0;
				i2_r = 0;
				i3_r = 0;
				k1_r = 0;
				k2_r = 0;
				k3_r = 0;
				e1_r = 0;
				e2_r = 0;
				e3_r = 0;
				e4_r = 0;
				p1_r = 0;
				p2_r = 0;
				for (int i = 0; i < 12; i++) {
					compFlag[i] = false;
				}
				for (int i = 0; i < 1000; i++) {
					squarePos[i][0] = 0;
					squarePos[i][1] = 0;
					squarePos[i][2] = 0;
				}
				index = 0;
				endGet = false;
			}
		}
		
		// End the scene
		g_pd3dDevice->EndScene();
	}

	// Present the backbuffer contents to the display
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		float width, height;
		width = lParam & 0xFFFF;
		height = (lParam >> 16) & 0xFFFF;
		g_aspect = width / height;
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}




//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
	pretime = timeGetTime();
	for (int i = 0; i<12; i++) {
		compFlag[i] = false;
	}

	UNREFERENCED_PARAMETER(hInst);

	// Register the window class
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"IKEP_logo", NULL
	};
	RegisterClassEx(&wc);

	// Create the application's window
	HWND hWnd = CreateWindow(L"IKEP_logo", L"IKEP",
		WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800,
		NULL, NULL, wc.hInstance, NULL);

	// Initialize Direct3D
	if (SUCCEEDED(InitD3D(hWnd)))
	{
		// Create the geometry
		if (SUCCEEDED(InitGeometry()))
		{
			// Show the window
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);

			// Enter the message loop
			MSG msg;
			ZeroMemory(&msg, sizeof(msg));
			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
					Render();
			}
		}
	}

	UnregisterClass(L"IKEP_logo", wc.hInstance);
	return 0;
}



