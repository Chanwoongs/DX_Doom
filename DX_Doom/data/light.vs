////////////////////////////////////////////////////////////////////////////////
// Filename: light.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBuffer
{
    float3 cameraPosition;
	float padding; // GPU�� �Ѱ��ٶ� 16byte ������ �ֱ�����
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 instancePosition : TEXCOORD1;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output;
	float4 worldPosition;
    float angle = 0.0f;
    
    matrix <float, 4, 4> rotMatrix = { cos(angle), 0.0f, sin(angle), 0.0f,
                  0.0f, 1.0f, 0.0f, 0.0f,
                  -sin(angle), 0.0f, cos(angle), 0.0f,
                  0.0f, 0.0f, 0.0f, 1.0f
                  };

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    input.position.x += input.instancePosition.x;
    input.position.y += input.instancePosition.y;
    input.position.z += input.instancePosition.z;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, mul(rotMatrix, worldMatrix));
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;
    
	// Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)mul(rotMatrix, worldMatrix));
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

	// Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, mul(rotMatrix, worldMatrix)); // Vertex�� world ��ǥ��� �ٲ�

    // Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
    output.viewDirection = cameraPosition.xyz - worldPosition.xyz; // E�� ���Ѵ�. ī�޶� ��ġ - Vertex
	
    // Normalize the viewing direction vector.
    output.viewDirection = normalize(output.viewDirection);

    return output;
}