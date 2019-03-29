#pragma once
#include "ISingleton.h"
#include "ScrollBar.h"
#include "MyButton.h"

enum class EConfig : char 
{
	MaxFrame = 0,
	GameSpeed,
	Volume,
	Close,
	Reset,
};


class Settings : public MyUI, public ISingleton<Settings>
{
protected:
	map<EConfig, ScrollBar*>		  m_ScrollBars;
	map<EConfig, MyButton<Settings>*> m_Buttons;
private:
	friend class ISingleton<Settings>;
	Settings() = default;
public:
	virtual ~Settings() = default;

	virtual MyActor* clone();

	void ResetConfig();
	void CloseConfig();
	virtual void MouseDragEvent() override;

	//virtual bool Init()		noexcept override;
	bool Init(const wstring_view& myName, const EObjType& objType,
			  const string_view& vertexShaderName = "VS_Basic", const string_view& pixelShaderName = "PShader") noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()	noexcept override;
};