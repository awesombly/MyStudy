#pragma once

enum class ETextureType : char {
	Diffuse, CubeMap, NormalMap, HeightMap, 
	Effect, Effect3D, Sprite, Etc,
};

struct TexItem {
	wstring  Name;
	Texture* pTexture;
	ETextureType eTexType;
};


class Dialog_CRenderer : public CFormView
{
public:
	static Dialog_CRenderer* Instance;

	CListBox  m_TextureList;
	map<ETextureType, list<TexItem> > m_TexItemMap;

	CButton m_isEnable;

	CButton m_onDiffuse;
	CButton m_onCubeMap;
	CButton m_onNormalMap;
	CButton m_onHeightMap;
	CButton m_onEffect;
	CButton m_onSprite;
	CButton m_onEffect3D;
	CButton m_onEtc;

	CEdit m_HeightCellSize;
	CEdit m_OffsetX;
	CEdit m_OffsetY;
	CEdit m_StartOffsetX;
	CEdit m_StartOffsetY;
	CEdit m_SpriteFrame;
	CEdit m_SpriteCount;


	CButton m_NormalApply;
	CButton m_NormalRelease;
	CButton m_HeightApply;
	CButton m_HeightRelease;
	CButton m_EnviApply;
	CButton m_EnviRelease;
	CComboBox m_EnviType;

	CEdit m_ColorX;
	CEdit m_ColorY;
	CEdit m_ColorZ;
	CEdit m_ColorW;

	CEdit m_LightRate;
	CEdit m_ShadowRate;
	CButton m_LightApply;
	CButton m_ShadowApply;

	bool m_dontUpdate = false;
	CString m_tempString;
	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CRenderer };
#endif
	static Dialog_CRenderer* CreateOne(CWnd* pParent);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
private:
	void UpdateEditEnable(const ERenderType& eRendType) noexcept;
	void LoadTextureList()						noexcept;
public:
	void LoadSpriteList()						noexcept;
	void InitTextureList()						noexcept;
	void UpdateEdit(const Component* pComp)		noexcept;
	void UpdateTextureList()					noexcept;
	void ApplyHeightMap() noexcept;
public:
	DECLARE_DYNAMIC(Dialog_CRenderer)
	Dialog_CRenderer();   // 표준 생성자입니다.
	virtual ~Dialog_CRenderer();
	afx_msg void OnBnClickedPisenable();
	afx_msg void OnBnClickedRondiffuse();
	afx_msg void OnBnClickedRoncubemap();
	afx_msg void OnBnClickedRonnormalmap();
	afx_msg void OnBnClickedRoneffect();
	afx_msg void OnBnClickedRonsprite();
	afx_msg void OnBnClickedRoneffect3d();
	afx_msg void OnBnClickedRonheightmap();
	afx_msg void OnBnClickedRonetc();	// etc..

	afx_msg void OnBnClickedPresetload();

	afx_msg void OnBnClickedRDiffuseapply();
	afx_msg void OnBnClickedRNormalapply();
	afx_msg void OnBnClickedREnviapply();

	afx_msg void OnBnClickedRDiffusereset();
	afx_msg void OnBnClickedRNormalreset();
	afx_msg void OnBnClickedREnvireset();

	afx_msg void OnCbnSelchangeREnvitype();
	afx_msg void OnEnChangeRColorx();
	afx_msg void OnEnChangeRColory();
	afx_msg void OnEnChangeRColorz();
	afx_msg void OnEnChangeRColorw();
	afx_msg void OnBnClickedREnviapply2();
	afx_msg void OnBnClickedREnvireset2();
	afx_msg void OnBnClickedRDiffuseapply2();
	afx_msg void OnBnClickedRLightapply();
	afx_msg void OnBnClickedRShadowapply();
};