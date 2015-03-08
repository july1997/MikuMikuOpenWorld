///DXライブラリ用のbulletデバッグドロー

#include "_dxdebugdraw.h"

//コンストラクタ
DxDebugDraw::DxDebugDraw()
:m_debugMode(0)
{

}

void DxDebugDraw::drawLine(const btVector3& from,const btVector3& to,const btVector3& fromColor, const btVector3& toColor)
{
	if (VSize(VAdd(limpos, btVGet(from)))<500)
	DrawLine3D(btVGet(from), btVGet(to), 
		GetColor( int(fromColor.getX()*255), int(fromColor.getY()*255), int(fromColor.getZ()*255) )
		);
}

void DxDebugDraw::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
{
	drawLine(from,to,color,color);
}

void DxDebugDraw::drawSphere (const btVector3& p, btScalar radius, const btVector3& color)
{
	DrawSphere3D(btVGet(p), radius, 10, 
		GetColor( int(color.getX()*255), int(color.getY()*255), int(color.getZ()*255) )
		, 0xFFFFFF, TRUE);
}

void DxDebugDraw::drawBox (const btVector3& boxMin, const btVector3& boxMax, const btVector3& color, btScalar alpha)
{
//	btVector3 halfExtent = (boxMax - boxMin) * btScalar(0.5f);
//	btVector3 center = (boxMax + boxMin) * btScalar(0.5f);
//	//glEnable(GL_BLEND);     // Turn blending On
//	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//	glColor4f (color.getX(), color.getY(), color.getZ(), alpha);
//	glPushMatrix ();
//	glTranslatef (center.getX(), center.getY(), center.getZ());
//	glScaled(2*halfExtent[0], 2*halfExtent[1], 2*halfExtent[2]);
////	glutSolidCube(1.0);
//	glPopMatrix ();
//	//glDisable(GL_BLEND);
}

void	DxDebugDraw::drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha)
{
	DrawTriangle3D(btVGet(a), btVGet(b), btVGet(c), 
		GetColor( int(color.getX()*255), int(color.getY()*255), int(color.getZ()*255) )
		, TRUE);
}

void	DxDebugDraw::setDebugMode(int debugMode)
{
	m_debugMode = debugMode;
}

void	DxDebugDraw::draw3dText(const btVector3& location,const char* textString)
{
	//glRasterPos3f(location.x(),  location.y(),  location.z());
	//BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),textString);
}

void	DxDebugDraw::reportErrorWarning(const char* warningString)
{
	printfDx("%s\n",warningString);
}

void	DxDebugDraw::drawContactPoint(const btVector3& pointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
{
	VECTOR from = btVGet(pointOnB);
	VECTOR to = VAdd(from, btVGet(normalOnB));
	DrawLine3D(from, to, 0x0000FF);
}
