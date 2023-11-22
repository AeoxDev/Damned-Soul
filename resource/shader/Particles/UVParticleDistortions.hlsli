
// Function to UV Pan coordinates according to constant buffer values and based on time.
float2 UVPan(in float2 offsetXY, in float panSpeed, in float time, in float2 originalUVCoordinates, out float2 pannedUVCoordinates)
{
    pannedUVCoordinates.x = originalUVCoordinates.x - frac(offsetXY.x * panSpeed * time);
    pannedUVCoordinates.y = originalUVCoordinates.y - frac(offsetXY.y * panSpeed * time);

    return pannedUVCoordinates;
}
// Overloaded, in case you don't need to also receive a variable out for the new uv coordinates.
float2 UVPan(in float2 offsetXY, in float panSpeed, in float time, in float2 originalUVCoordinates)
{
    return float2(originalUVCoordinates.x - frac(offsetXY.x * panSpeed * time),
                originalUVCoordinates.y - frac(offsetXY.y * panSpeed * time));
}