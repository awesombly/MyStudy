// setShaderResourceView~ �Ҷ� ù��° ���ڰ� ���������� �����̰�,
// ���������� �� ���Կ� �ش��ϴ� �ؽ��� �ȼ� ���� �޾ƿ� ������ ����� �� �ִ�.(Max 8, ���÷��� ��������)

Texture2D g_txDiffuse :  register (t0);
//Texture2D g_txDiffuse2 :  register (t1);
SamplerState samLinear : register (s0);

struct VS_INPUT
{
	float3 p : POSITION;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};
struct VS_INPUT_PNCT
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};

VS_OUTPUT VShader(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.p = float4(input.p, 1.0f);
	output.c = input.c;
	output.t = input.t;
	return output;
}

VS_OUTPUT VS_Basic(VS_INPUT_PNCT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.p = float4(input.p, 1.0f);
	output.c = input.c;
	output.t = input.t;
	return output;
}

float4 PShader(VS_OUTPUT input) : SV_TARGET
{
	return g_txDiffuse.Sample(samLinear, input.t) * input.c;
}

float4 PPureColor(VS_OUTPUT input) : SV_TARGET
{
	return input.c;
}

float4 PButtonOver(VS_OUTPUT input) : SV_TARGET
{
	return g_txDiffuse.Sample(samLinear, input.t) * (input.c * 1.25f);
}

float4 PButtonClick(VS_OUTPUT input) : SV_TARGET
{
	return g_txDiffuse.Sample(samLinear, input.t) * (input.c * 0.75f);
}

float4 PHalfAlpha(VS_OUTPUT input) : SV_TARGET
{
	return g_txDiffuse.Sample(samLinear, input.t) * input.c * 0.55;
}

float4 PRedColor(VS_OUTPUT input) : SV_TARGET
{
	return g_txDiffuse.Sample(samLinear, input.t) + float4(1.0f, -0.6f, -0.6f, 0.0f);
}

//float4 PNoneColor(VS_OUTPUT input) : SV_TARGET
//{
//	return g_txDiffuse.Sample(samLinear, input.t);
//}