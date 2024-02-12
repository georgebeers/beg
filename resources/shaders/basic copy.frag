#version 450 core

in vec3 vertNormal;
in vec3 vertColor;
in vec3 fragPosition;

out vec4 FragColor;

uniform vec3 viewPosition;

uniform bool lightable;

uniform struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
} material;

uniform int numberOfDirectionalLights;
uniform struct DirectionalLight {
    vec3 direction;
    vec3 color;

    float ambientStrength;
} directionalLights[32];

uniform int numberOfPointLights;
uniform struct PointLight {
    vec3 position;
    vec3 color;

    float radius;
    float ambientStrength;
} pointLights[32];

uniform int numberOfSpotLights;
uniform struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 color;

    float range;
    float angle;
    float blurAngle;
} spotLights[32];

vec3 CalculateDirectionalLight(DirectionalLight light, Material material, vec3 normal, vec3 viewPosition) {
    ambient += light.color * light.ambientStrength * material.ambient;

    float diff = max(dot(normal, light.direction), 0.0)
    diffuse += light.color * (diff * material.diffuse)

    vec3 viewDir = normalize(viewPosition - fragPosition);
    vec3 reflectDir = reflect(-light.direction, vertNormal)

    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess)
    specular += light.color * (spec * material.specular);

    return (ambient + diffuse + specular)
}

void main() {
    vec3 result = vertColor;
    if (lightable) {
        vec3 ambient = vec3(0.0f);
        vec3 diffuse = vec3(0.0f);
        vec3 specular = vec3(0.0f);

        for (int i = 0; i < numberOfDirectionalLights; ++i) {
            ambient += directionalLights[i].color * directionalLights[i].ambientStrength * material.ambient;

            float diff = max(dot(vertNormal, directionalLights[i].direction), 0.0);

            diffuse += directionalLights[i].color * (diff * material.diffuse);

            vec3 viewDir = normalize(viewPosition - fragPosition);
            vec3 reflectDir = reflect(-directionalLights[i].direction, vertNormal);

            float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

            specular += directionalLights[i].color * (spec * material.specular);
        }

        for (int i = 0; i < numberOfPointLights; ++i) {
            float dist = length(pointLights[i].position - fragPosition);
            float attenuation = 1.0f / (pow(dist / pointLights[i].radius, 2.0f) + 1.0f);

            ambient += (pointLights[i].color * pointLights[i].ambientStrength * material.ambient) * attenuation;

            vec3 lightDir = normalize(pointLights[i].position - fragPosition);

            float diff = max(dot(vertNormal, lightDir), 0.0);

            diffuse += (pointLights[i].color * (diff * material.diffuse)) * attenuation;

            vec3 viewDir = normalize(viewPosition - fragPosition);
            vec3 reflectDir = reflect(-lightDir, vertNormal);

            float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

            specular += (pointLights[i].color * (spec * material.specular)) * attenuation;
        }

        for (int i = 0; i < numberOfSpotLights; ++i) {
            vec3 lightDir = normalize(spotLights[i].position - fragPosition);

            float theta = dot(lightDir, normalize(-spotLights[i].direction));
            
            if (theta > cos(spotLights[i].angle)) {
                float epsilon = cos(spotLights[i].angle - spotLights[i].blurAngle) - cos(spotLights[i].angle);
                float intensity = clamp((theta - cos(spotLights[i].angle)) / epsilon, 0.0f, 1.0f);

                float dist = length(spotLights[i].position - fragPosition);
                float attenuation = 1.0f / (pow(dist / spotLights[i].range, 2.0f) + 1.0f);

                float diff = max(dot(vertNormal, lightDir), 0.0);

                diffuse += (spotLights[i].color * (diff * material.diffuse)) * attenuation * intensity;

                vec3 viewDir = normalize(viewPosition - fragPosition);
                vec3 reflectDir = reflect(-lightDir, vertNormal);

                float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

                specular += (spotLights[i].color * (spec * material.specular)) * attenuation * intensity;
            }
        }

        result = (ambient + diffuse + specular) * vertColor;
    }

    FragColor = vec4(result, 1.0f);
}