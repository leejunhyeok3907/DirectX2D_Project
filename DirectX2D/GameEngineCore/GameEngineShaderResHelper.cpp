#include "PreCompile.h"
#include "GameEngineShaderResHelper.h"
#include "GameEngineShader.h"


void GameEngineConstantBufferSetter::Setting()
{
	if (nullptr == CPUDataPtr)
	{
		MsgBoxAssert(Name + "상수버퍼를 세팅해주지 않았습니다.");
	}

	ShaderType Type = ParentShader->GetShaderType();

	Res->ChangeData(CPUDataPtr, DataSize);

	switch (Type)
	{
	case ShaderType::Vertex:
		Res->VSSetting(BindPoint);
		break;
	case ShaderType::Pixel:
		Res->PSSetting(BindPoint);
		break;
	default:
		MsgBoxAssert("처리할수 없는 쉐이더 세팅 유형입니다.");
		break;
	}

}

void GameEngineConstantBufferSetter::Reset()
{

}

void GameEngineTextureSetter::Setting()
{
	ShaderType Type = ParentShader->GetShaderType();

	switch (Type)
	{
	case ShaderType::Vertex:
		Res->VSSetting(BindPoint);
		break;
	case ShaderType::Pixel:
		Res->PSSetting(BindPoint);
		break;
	default:
		MsgBoxAssert("처리할수 없는 쉐이더 세팅 유형입니다.");
		break;
	}

}

void GameEngineTextureSetter::Reset()
{

}

void GameEngineSamplerSetter::Setting()
{
	ShaderType Type = ParentShader->GetShaderType();

	switch (Type)
	{
	case ShaderType::Vertex:
		Res->VSSetting(BindPoint);
		break;
	case ShaderType::Pixel:
		Res->PSSetting(BindPoint);
		break;
	default:
		MsgBoxAssert("처리할수 없는 쉐이더 세팅 유형입니다.");
		break;
	}

}

void GameEngineSamplerSetter::Reset()
{

}


GameEngineShaderResHelper::GameEngineShaderResHelper()
{
}

GameEngineShaderResHelper::~GameEngineShaderResHelper()
{
}



void GameEngineShaderResHelper::ShaderResCheck(std::string _FunctionName, GameEngineShader* _Shader, ID3DBlob* _CompileCode)
{
	if (nullptr == _CompileCode)
	{
		MsgBoxAssert("컴파일 되지 않은 코드로는 쉐이더의 리소스를 조사할수가 없습니다.");
		return;
	}

	ID3DBlob* BinaryCode = _CompileCode;

	// 리플렉션을 통해서 얻어내는 것은 코드 그자체의 정보를 의미한다.
	// 클래스 개수
	// 함수
	// 클래스의 타입
	// 상속은 받았느니? 
	ID3D11ShaderReflection* CompileInfo = nullptr;

	if (S_OK != D3DReflect(BinaryCode->GetBufferPointer(), BinaryCode->GetBufferSize(), IID_ID3D11ShaderReflection, reinterpret_cast<void**>(&CompileInfo)))
	{
		MsgBoxAssert("쉐이더 정보수집에 실패했습니다.");
		return;
	}

	D3D11_SHADER_DESC Info;

	// Info
	// 엔진 => 딸깍
	// 엔진에는 이미 갖춰진 데다가 + 노드가 된다. 언리얼 같은 경우에는 
	CompileInfo->GetDesc(&Info);

	D3D11_SHADER_INPUT_BIND_DESC ResDesc;
	// setTexture("MyTexture", "aaaa.png");


	// 텍스처 쉐이더 샘프러 다 합쳐서 몇개냐?
	for (UINT i = 0; i < Info.BoundResources; i++)
	{
		// 텍스처를 10개썼다 10
		// 텍스처 2개 상수버퍼 3개썼다. 
		CompileInfo->GetResourceBindingDesc(i, &ResDesc);

		std::string UpperName = GameEngineString::ToUpperReturn(ResDesc.Name);



		D3D_SHADER_INPUT_TYPE Type = ResDesc.Type;
		//D3D_SIT_CBUFFER = 0,
		//D3D_SIT_TBUFFER = (D3D_SIT_CBUFFER + 1),
		//D3D_SIT_TEXTURE = (D3D_SIT_TBUFFER + 1),
		//D3D_SIT_SAMPLER = (D3D_SIT_TEXTURE + 1),
		//D3D_SIT_UAV_RWTYPED = (D3D_SIT_SAMPLER + 1),
		//D3D_SIT_STRUCTURED = (D3D_SIT_UAV_RWTYPED + 1),
		//D3D_SIT_UAV_RWSTRUCTURED = (D3D_SIT_STRUCTURED + 1),
		//D3D_SIT_BYTEADDRESS = (D3D_SIT_UAV_RWSTRUCTURED + 1),
		//D3D_SIT_UAV_RWBYTEADDRESS = (D3D_SIT_BYTEADDRESS + 1),
		//D3D_SIT_UAV_APPEND_STRUCTURED = (D3D_SIT_UAV_RWBYTEADDRESS + 1),
		//D3D_SIT_UAV_CONSUME_STRUCTURED = (D3D_SIT_UAV_APPEND_STRUCTURED + 1),
		//D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER = (D3D_SIT_UAV_CONSUME_STRUCTURED + 1),
		//D3D_SIT_RTACCELERATIONSTRUCTURE = (D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER + 1),
		//D3D_SIT_UAV_FEEDBACKTEXTURE = (D3D_SIT_RTACCELERATIONSTRUCTURE + 1),
		//D3D10_SIT_CBUFFER = D3D_SIT_CBUFFER,
		//D3D10_SIT_TBUFFER = D3D_SIT_TBUFFER,
		//D3D10_SIT_TEXTURE = D3D_SIT_TEXTURE,
		//D3D10_SIT_SAMPLER = D3D_SIT_SAMPLER,
		//D3D11_SIT_UAV_RWTYPED = D3D_SIT_UAV_RWTYPED,
		//D3D11_SIT_STRUCTURED = D3D_SIT_STRUCTURED,
		//D3D11_SIT_UAV_RWSTRUCTURED = D3D_SIT_UAV_RWSTRUCTURED,
		//D3D11_SIT_BYTEADDRESS = D3D_SIT_BYTEADDRESS,
		//D3D11_SIT_UAV_RWBYTEADDRESS = D3D_SIT_UAV_RWBYTEADDRESS,
		//D3D11_SIT_UAV_APPEND_STRUCTURED = D3D_SIT_UAV_APPEND_STRUCTURED,
		//D3D11_SIT_UAV_CONSUME_STRUCTURED = D3D_SIT_UAV_CONSUME_STRUCTURED,
		//D3D11_SIT_UAV_RWSTRUCTURED_WITH_COUNTER = D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER

		switch (Type)
		{
		case D3D_SIT_CBUFFER:
		{
			ID3D11ShaderReflectionConstantBuffer* BufferInfo = CompileInfo->GetConstantBufferByName(ResDesc.Name);

			D3D11_SHADER_BUFFER_DESC BufferDesc;
			BufferInfo->GetDesc(&BufferDesc);

			// 당연히 상수버퍼를 만들어야 합니다.
			std::shared_ptr<GameEngineConstantBuffer> CBuffer = GameEngineConstantBuffer::CreateAndFind(BufferDesc.Size, UpperName, BufferDesc);

			_FunctionName;
			GameEngineConstantBufferSetter NewSetter;
			NewSetter.ParentShader = _Shader;
			NewSetter.Res = CBuffer;
			NewSetter.Name = UpperName;
			NewSetter.BindPoint = ResDesc.BindPoint;
			NewSetter.DataSize = BufferDesc.Size;

			ConstantBufferSetters.insert(std::make_pair(UpperName, NewSetter));

			break;
		}
		case D3D_SIT_TEXTURE:
		{
			std::shared_ptr<GameEngineTexture> Res
				= GameEngineTexture::Find("NSet.png");

			_FunctionName;
			GameEngineTextureSetter NewSetter;
			NewSetter.ParentShader = _Shader;
			NewSetter.Res = Res;
			NewSetter.Name = UpperName;
			NewSetter.BindPoint = ResDesc.BindPoint;

			TextureSetters.insert(std::make_pair(UpperName, NewSetter));
			break;
		}
		case D3D_SIT_SAMPLER:
		{
			//std::shared_ptr<GameEngineSampler> Res
			//	= GameEngineSampler::Find("EngineBaseSampler");

			// 이녀석은 세팅 해주지 않으면 터진다.
			_FunctionName;
			GameEngineSamplerSetter NewSetter;
			NewSetter.ParentShader = _Shader;
			NewSetter.Res = nullptr;
			NewSetter.Name = UpperName;
			NewSetter.BindPoint = ResDesc.BindPoint;

			SamplerSetters.insert(std::make_pair(UpperName, NewSetter));
			break;
		}
		default:
			break;
		}
	}


}

void GameEngineShaderResHelper::ShaderResCopy(GameEngineShader* _Shader)
{
	std::multimap<std::string, GameEngineConstantBufferSetter>& OtherConstantBufferSetters = _Shader->ResHelper.ConstantBufferSetters;
	std::multimap<std::string, GameEngineTextureSetter>& OtherTextureSetters = _Shader->ResHelper.TextureSetters;
	std::multimap<std::string, GameEngineSamplerSetter>& OtherSamplerSetters = _Shader->ResHelper.SamplerSetters;

	for (std::pair<const std::string, GameEngineConstantBufferSetter>& Pair : OtherConstantBufferSetters)
	{
		ConstantBufferSetters.insert(std::make_pair(Pair.first, Pair.second));
	}

	for (std::pair<const std::string, GameEngineTextureSetter>& Pair : OtherTextureSetters)
	{
		TextureSetters.insert(std::make_pair(Pair.first, Pair.second));
	}

	for (std::pair<const std::string, GameEngineSamplerSetter>& Pair : OtherSamplerSetters)
	{
		SamplerSetters.insert(std::make_pair(Pair.first, Pair.second));
	}

	// 기본 샘플러로 세팅해줘야할 녀석들이 있는지 확인한다.
	for (std::pair<const std::string, GameEngineTextureSetter>& Pair : OtherTextureSetters)
	{
		std::string SamplerName = Pair.first + "SAMPLER";

		if (true == IsSampler(SamplerName))
		{
			std::shared_ptr<GameEngineSampler> Sampler = Pair.second.Res->GetBaseSampler();

			SetSampler(SamplerName, Sampler);
		}
	}
}

void GameEngineShaderResHelper::AllShaderResourcesSetting()
{
	for (std::pair<const std::string, GameEngineConstantBufferSetter>& Pair : ConstantBufferSetters)
	{
		if (nullptr == Pair.second.Res)
		{
			MsgBoxAssert(std::string(Pair.first) + "라는 샘플러가 세팅이 되지 않았습니다.");
			return;
		}

		Pair.second.Setting();
	}

	for (std::pair<const std::string, GameEngineTextureSetter>& Pair : TextureSetters)
	{
		if (nullptr == Pair.second.Res)
		{
			MsgBoxAssert(std::string(Pair.first) + "라는 샘플러가 세팅이 되지 않았습니다.");
			return;
		}

		Pair.second.Setting();
	}

	for (std::pair<const std::string, GameEngineSamplerSetter>& Pair : SamplerSetters)
	{
		if (nullptr == Pair.second.Res)
		{
			MsgBoxAssert(std::string(Pair.first) + "라는 샘플러가 세팅이 되지 않았습니다.");
			return;
		}

		Pair.second.Setting();
	}

}

void GameEngineShaderResHelper::SetConstantBufferLink(std::string_view _Name, const void* _Data, size_t _Size)
{
	if (false == IsConstantBuffer(_Name))
	{
		MsgBoxAssert(std::string(_Name) + "존재하지 않는 상수버퍼에 링크를 걸려고 했습니다.");
		return;
	}

	std::string UpperString = GameEngineString::ToUpperReturn(_Name);

	// 중복되는 이름의 시작 이터레이터와 끝 이터레이터를 찾는법
	std::multimap<std::string, GameEngineConstantBufferSetter>::iterator NameStariter
		= ConstantBufferSetters.lower_bound(UpperString);
	std::multimap<std::string, GameEngineConstantBufferSetter>::iterator NameEnditer
		= ConstantBufferSetters.upper_bound(UpperString);

	for (; NameStariter != NameEnditer; ++NameStariter)
	{
		GameEngineConstantBufferSetter& Setter = NameStariter->second;
		if (Setter.DataSize != _Size)
		{
			MsgBoxAssert(NameStariter->first + "상수버퍼에 크기가 다른 데이터를 세팅하려고 했습니다.");
		}

		Setter.CPUDataPtr = _Data;
	}

}


void GameEngineShaderResHelper::SetTexture(std::string_view _Name, std::string_view _TextureName)
{
	std::shared_ptr<GameEngineTexture> Tex = GameEngineTexture::Find(_TextureName);
	if (nullptr == Tex)
	{
		MsgBoxAssert("존재하지 않는 텍스처 입니다.");
	}

	SetTexture(_Name, Tex);
}

void GameEngineShaderResHelper::SetTexture(std::string_view _Name, std::shared_ptr<GameEngineTexture> _Texture)
{
	if (false == IsTexture(_Name))
	{
		MsgBoxAssert(std::string(_Name) + "존재하지 않는 텍스터 세팅에 링크를 걸려고 했습니다.");
		return;
	}

	std::string UpperString = GameEngineString::ToUpperReturn(_Name);

	// 중복되는 이름의 시작 이터레이터와 끝 이터레이터를 찾는법
	std::multimap<std::string, GameEngineTextureSetter>::iterator NameStariter
		= TextureSetters.lower_bound(UpperString);
	std::multimap<std::string, GameEngineTextureSetter>::iterator NameEnditer
		= TextureSetters.upper_bound(UpperString);

	std::string SamplerName = NameStariter->first + "SAMPLER";

	for (; NameStariter != NameEnditer; ++NameStariter)
	{
		GameEngineTextureSetter& Setter = NameStariter->second;

		Setter.Res = _Texture;

		if (true == IsSampler(SamplerName))
		{
			std::shared_ptr<GameEngineSampler> Sampler = Setter.Res->GetBaseSampler();

			SetSampler(SamplerName, Sampler);
		}
	}
}

void GameEngineShaderResHelper::SetSampler(std::string_view _Name, std::shared_ptr<GameEngineSampler> _TextureSampler)
{
	if (false == IsSampler(_Name))
	{
		MsgBoxAssert("존재하지 않는 상수버퍼에 링크를 걸려고 했습니다.");
		return;
	}

	std::string UpperString = GameEngineString::ToUpperReturn(_Name);

	// 중복되는 이름의 시작 이터레이터와 끝 이터레이터를 찾는법
	std::multimap<std::string, GameEngineSamplerSetter>::iterator NameStariter
		= SamplerSetters.lower_bound(UpperString);
	std::multimap<std::string, GameEngineSamplerSetter>::iterator NameEnditer
		= SamplerSetters.upper_bound(UpperString);

	for (; NameStariter != NameEnditer; ++NameStariter)
	{
		GameEngineSamplerSetter& Setter = NameStariter->second;

		Setter.Res = _TextureSampler;
	}
}

void GameEngineShaderResHelper::ResClear()
{
	ConstantBufferSetters.clear();
	TextureSetters.clear();
	SamplerSetters.clear();
}