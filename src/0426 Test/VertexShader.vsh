// Vertex Shader - 정점 1개 단위로 처리, High Level Shader Language

float4 VShader( in float4 _point : POSITION) : SV_POSITION
{
	return _point;
}