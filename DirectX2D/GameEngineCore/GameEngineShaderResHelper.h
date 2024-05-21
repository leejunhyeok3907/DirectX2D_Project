#pragma once
#include "GameEngineConstantBuffer.h"
#include "GameEngineTexture.h"
#include "GameEngineSampler.h"

class GameEngineShaderResources
{
public:
	std::string Name; // Transformdata <= 쉐이더에서 사용하는 리소스의 이름
	class GameEngineShader* ParentShader; // <= 날 사용하는 쉐이더
	int BindPoint = -1; // <= b0 t0 s0 ... s1 s2 s3, b1 b2 b3 몇번 상수버퍼냐

public:
	virtual void Setting() = 0;
	virtual void Reset() = 0;
};

// 이 쉐이더가 상수버퍼를 몇개 사용하냐
// 상수버퍼가 아니라.
// 상수버퍼를 이용하는 클래스
class GameEngineConstantBufferSetter : public GameEngineShaderResources
{
public:
	std::shared_ptr<GameEngineConstantBuffer> Res;

	const void* CPUDataPtr = nullptr;
	UINT DataSize = -1;

	void Setting() override;
	void Reset() override;
};

class GameEngineTextureSetter : public GameEngineShaderResources
{
public:
	std::shared_ptr<GameEngineTexture> Res;

	void Setting() override;
	void Reset() override;
};

class GameEngineSamplerSetter : public GameEngineShaderResources
{
public:
	std::shared_ptr<GameEngineSampler> Res;

	void Setting() override;
	void Reset() override;
};

// 설명 : 쉐이더 리소스 헬퍼는 2가지 역할을 합니다.
//       1. 특정 쉐이더가 어떤 리소스들을 가지고 있느냐를 조사를 해주는 역할을
//       2. 특정 랜더러가 어떤 리소스들을 세팅해야하는지에 대한 역할 <= 멀티맵으로 해야한다.

// 이걸 만들게된 이유는 <= 그런 복잡한 리소스의 구조를 이름으로 통합하기 위해서 
// SetConstBuffer(0, TransData);
// SetConstBuffer("TransformData", TransData);

class GameEngineShaderResHelper
{
public:
	// constrcuter destructer
	GameEngineShaderResHelper();
	~GameEngineShaderResHelper();

	// 쉐이더의 컴파일된 코드 결과물
	void ShaderResCheck(std::string _FunctionName, class GameEngineShader* _Shader, ID3DBlob* _CompileCode);

	void ShaderResCopy(class GameEngineShader* _Shader);

	void AllShaderResourcesSetting();

	bool IsConstantBuffer(std::string_view _Name)
	{
		std::string UpperString = GameEngineString::ToUpperReturn(_Name);

		return ConstantBufferSetters.contains(UpperString);
	}

	bool IsTexture(std::string_view _Name)
	{
		std::string UpperString = GameEngineString::ToUpperReturn(_Name);

		return TextureSetters.contains(UpperString);
	}

	bool IsSampler(std::string_view _Name)
	{
		std::string UpperString = GameEngineString::ToUpperReturn(_Name);

		return SamplerSetters.contains(UpperString);
	}

	// 여기에 값형만 들어갑니다.
	template<typename DataType>
	void SetConstantBufferLink(std::string_view _Name, const DataType& _Data)
	{
		SetConstantBufferLink(_Name, &_Data, sizeof(_Data));
	}

	void SetConstantBufferLink(std::string_view _Name, const void* _Data, size_t _Size);

	void SetTexture(std::string_view _Name, std::string_view _TextureName);

	void SetTexture(std::string_view _Name, std::shared_ptr<GameEngineTexture> _Texture);

	void SetSampler(std::string_view _Name, std::shared_ptr<GameEngineSampler> _TextureSampler);

	void ResClear();

protected:

private:
	// std::shared_ptr로 만들고 
	// 그걸 기억을 해놔야하기 때문에.
	std::multimap<std::string, GameEngineConstantBufferSetter> ConstantBufferSetters;
	std::multimap<std::string, GameEngineTextureSetter> TextureSetters;
	std::multimap<std::string, GameEngineSamplerSetter> SamplerSetters;
};

