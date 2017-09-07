#include "stdafx.h"
#include "D3DUtility.h"

namespace D3DUTILITY
{

	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
	{
		D3DMATERIAL9 mtrl;
		mtrl.Ambient = a;
		mtrl.Diffuse = d;
		mtrl.Specular = s;
		mtrl.Emissive = e;
		mtrl.Power = p;

		return mtrl;
	}

	void ComputeNormal(D3DXVECTOR3* out, D3DXVECTOR3* p0, D3DXVECTOR3* p1, D3DXVECTOR3* p2)
	{
		D3DXVECTOR3 v01 = *p1 - *p0;
		D3DXVECTOR3 v02 = *p2 - *p0;

		D3DXVec3Cross(out, &v01, &v02);
		D3DXVec3Normalize(out, out);
	}

	D3DLIGHT9 InitDirectional(D3DXVECTOR3* dir, D3DXCOLOR* c)
	{
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(D3DLIGHT9));

		light.Type = D3DLIGHT_DIRECTIONAL;
		light.Ambient = *c * 0.4f;
		light.Diffuse = *c;
		light.Specular = *c * 0.6f;
		light.Direction = *dir;

		return light;
	}

	D3DLIGHT9 InitPoint(D3DXVECTOR3* pos, D3DXCOLOR* c)
	{
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(D3DLIGHT9));

		light.Type = D3DLIGHT_POINT;
		light.Ambient = *c;
		light.Diffuse = *c;
		light.Specular = *c;
		light.Position = *pos;
		light.Range = 20.0f;
		light.Falloff = 1.0f;
		light.Attenuation0 = 0.005f;
		light.Attenuation1 = 0.005f;
		light.Attenuation2 = 0.005f;

		return light;
	}

	D3DLIGHT9 InitSpot(D3DXVECTOR3* dir, D3DXVECTOR3* pos, D3DXCOLOR* c)
	{
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(D3DLIGHT9));

		light.Type = D3DLIGHT_SPOT;
		light.Ambient = *c;
		light.Diffuse = *c;
		light.Specular = *c;
		light.Direction = *dir;
		light.Position = *pos;
		light.Range = 50.0f;
		light.Falloff = 1.0f;
		light.Theta = 0.0f;
		light.Phi = D3DX_PI / 4;
		light.Attenuation0 = 0.005f;
		light.Attenuation1 = 0.005f;
		light.Attenuation2 = 0.005f;

		return light;
	}
	float GetAngle(const D3DXVECTOR3 & _vStart, const D3DXVECTOR3 & _vDest,bool _toDegree/*false*/)
	{
		D3DXVECTOR3 vZero(0, 0, 0);
		if (_vDest == vZero)//목표 지점이 원점일때 예외처리
		{
			D3DXVec3Normalize(&vZero, &_vStart);
			float cosAngle = acosf(D3DXVec3Dot(&vZero,&D3DXVECTOR3(1,0,0)));
			float angle=(vZero.z > 0.f) ? cosAngle : -cosAngle;
			if (_toDegree)angle = D3DXToDegree(cosAngle);
			return angle;
		}

		float f = D3DXVec3Dot(&_vStart, &_vDest);
		float cosAngle = acosf(D3DXVec3Dot(&_vStart, &_vDest) / (D3DXVec3Length(&_vStart) * D3DXVec3Length(&_vDest)));
		
		//외적의 z성분으로 방향이 결정된다.
		float angle = (_vStart.x * _vDest.y - _vStart.y * _vDest.x > 0.0f) ? cosAngle : -cosAngle;
		if (_toDegree)angle = D3DXToDegree(cosAngle);
		return angle;
	}
}