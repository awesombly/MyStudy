#pragma once
#include "stdafx.h"
#include <map>

static constexpr float EPSILON = 1.0e-3f;

// ++ 애니메이션 1개만 있을때,
// ++ 엔터 줄이고, 바이너리화

enum class EClassType : char{
	Geom = 0, Biped,
	Bone, Dummy, 
};


struct D3D_MATRIX {
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;
		};
		float m[4][4];
	};
};


struct SceneData
{
	//wstring FileName;		// 파일 이름
	int FirstFrame = 0;		// 시작 프레임
	int LastFrame = 0;		// 끝 프레임
	//int FrameSpeed = 0;	// 초당 프레임 개수
	int TickPerFrame = 0;	// 1프레임 틱 값
	// ========== 출력부 ==========
	int TotalFrame = 0;		// 총 프레임 수
	float SPF = 0.0f;		// 프레임 간격
	int MeshCount;			// 메쉬오브젝트 개수
	int MaxWeight;			// 정점 당 가중치
	bool BindPose;			// 바인딩 포즈 에니메이션 여부
};

struct TexInfo {
	int Index;
	//int Type;
	wstring TexName;
};

struct MaterialData
{
	UINT	MaterialIndex;
	wstring MaterialName;
	wstring MaterialClassName;	// 클래스 타입(Standard, SubMaterial...)
	vector<MaterialData> SubMaterials;
	vector<TexInfo> TexInfoList;
	//Texture* pTexture = nullptr;
	//vector<Tex>;
	//UINT SubCount;
	//UINT TexMapCount;
	MaterialData* pParent = nullptr;
};


// 바이패드 메쉬?
struct Vertex_PNC {
	Point3 pos;
	Point3 nor;
	Point4 col;
};

struct Vertex_PNCT {
	Point3 pos;
	Point3 nor;
	Point4 col;
	Point2 tex;
	bool operator == (const Vertex_PNCT& vertex)
	{
		if (fabs(pos.x - vertex.pos.x) <= EPSILON &&
			fabs(pos.y - vertex.pos.y) <= EPSILON &&
			fabs(pos.z - vertex.pos.z) <= EPSILON &&
			fabs(nor.x - vertex.nor.x) <= EPSILON &&
			fabs(nor.y - vertex.nor.y) <= EPSILON && 
			fabs(nor.z - vertex.nor.z) <= EPSILON && 
			fabs(col.x - vertex.col.x) <= EPSILON && 
			fabs(col.y - vertex.col.y) <= EPSILON && 
			fabs(col.z - vertex.col.z) <= EPSILON && 
			fabs(col.w - vertex.col.w) <= EPSILON &&
			fabs(tex.x - vertex.tex.x) <= EPSILON &&
			fabs(tex.y - vertex.tex.y) <= EPSILON)
		{
			return true;
		}
		return false;
	}
};

struct AnimTrack {
	WORD Frame;
	Point3 vValue;
	Quat qValue;
};

struct RowObjectData {
	INode* pNode;
	Object* pObject;
	Control* pControl;

	EClassType ClassType;	
	int Index;			// 애니메이션 행렬 인덱스
	wstring Name;		// 이름
	//vector<Vertex_PNC> FaceList;	// 메쉬 페이스 리스트
	
	Matrix3 NodeTM;
	Matrix3 InvNodeTM;

	vector<AnimTrack> PosTrack;
	vector<AnimTrack> RotTrack;
	vector<AnimTrack> ScaleTrack;
};

struct Triangle {
	Vertex_PNCT ver[3];
	//int MatIndex;
};

struct AnimData
{
	Point3 position;
	Point3 scale;
	Quat   rotation;
	Matrix3 matScaleAxis;

	// Visivility Track?
	AnimData()
	{
		position = { 0.0f, 0.0f, 0.0f };
		scale = { 1.0f, 1.0f, 1.0f };
		rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
		matScaleAxis.IdentityMatrix();
	}
};

struct ObjectData {
	Box3		BoundBox;
	D3D_MATRIX	matLocal;
	Matrix3		matMaxLocal;
	Matrix3		NodeTM;
	//int		m_iNumVertex;
	//int		m_iNumFace;
	WORD		MaterialRef;

	wstring ObjName;
	wstring ParentName;
	EClassType ClassType;
	
	vector<Triangle>	Triangles;
	vector<Vertex_PNCT> VertexList;
	vector<UINT>		IndexList;

	vector<AnimData> Animations;
};


class GlobalEx
{
private:
	vector<Mtl*>			m_MtlList;		// 메테리얼 리스트
protected:
	Interface*  m_pInter;
	INode*		m_pRoot;
	Interval	m_Interval;

	SceneData	m_Scene;
	vector<MaterialData>	  m_Materials;	// 출력할 메테리얼
	map<wstring, RowObjectData> m_Matrixs;	// 초기 저장 데이터
	map<wstring, ObjectData>  m_Objects;	// 출력 오브젝트 리스트
protected:
	bool Init(Interface* pInter) noexcept;
	virtual void AnimationSetting() noexcept = 0;
	//int GetMaterialRef();
	//void SetBindPose(const bool& useBindPose = false);
	//int GetIndex(const wstring_view& nodeName);
private:
	void PreProcess(INode* pNode);
	void AddObject(INode* pNode);
	void AddMaterial(INode* pNode);

	void MtlToMaterials();
	void AddSubMaterial(MaterialData* pParent, Mtl* pMtl);
	void AddSubTexture(MaterialData* pParent, Mtl* pMtl);

	void MatToObjects();
	void SetObjectMesh(INode* pNode, ObjectData* pObject);
	TriObject*  GetTriObjectFromNode(INode* pNode, const TimeValue& t, bool* neenDelete);
	Point3		GetVertexNormal(Mesh* pMesh, const int& faceNo, RVertex* rv);
	void TriangleToVertexIndex();
protected:
	void TransD3DStruct(D3D_MATRIX* desc, const Matrix3& src);
	void TransD3DStruct(Matrix3* desc, const Matrix3& src);
	void TransD3DStruct(Point3* desc, const Point3& src);
	bool EqualPoint(const Point2& point1, const Point2& point2);
	bool EqualPoint(const Point3& point1, const Point3& point2);
	bool EqualPoint(const Point4& point1, const Point4& point2);
	bool EqualPoint(const Quat& point1, const Quat& point2);
	Point3 Lerp(const Point3& start, const Point3& end, const float& time);
	Quat Lerp(const Quat& start, const Quat& end, const float& time);
public:
	GlobalEx() = default;
	virtual ~GlobalEx() = default;
};