#version 440
layout(location = 0) in vec2 coord;
layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
    vec2 u_resolution;
    int currentLight;
    int num;
};

float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

float ball(vec2 uv){
    float col = 0.0;

    float l = length(uv);

    float size = 0.3;
    float falloff = 0.03;

    col = smoothstep(size + falloff, size, l);
    col += smoothstep(0.10, 0.858, 1.0-l) *  0.1 / pow(l * 2.152, 2.0);

    return clamp(col, 0.0, 1.0);
}

void main() {
    vec2 st = coord.xy - 0.5;
    st.x *= u_resolution.x / u_resolution.y;
    st *= max(480.0 / 640.0, u_resolution.y / u_resolution.x) * 640.0 / 480.0;


    float activeLight = float(currentLight);
    float n = float(num);

    float scale = 1.0;

    st.x += 1.0 / 2.0 / scale;
    st *= n * scale;
    st.x -= 0.5;


    vec2 fv;

    if (st.x > -1.0 && st.x < n+1.0){
        fv = vec2(fract(st.x), st.y);
    }
    else {
        fv = vec2(st.x - ((st.x < 0.0) ? 0.0 : n-1.0), st.y);
    }


    vec3 color = vec3(0.1);

    vec2 iv = vec2(floor(st.x), 0.0);

    float oneScale = 1.0;

    if (n == 1.0){
        fv *= 1.4;
        oneScale = 1.4;
    }

    for (int x = -1; x < 2; x++){

        if (x > 0 && iv.x <= -1.0){
            continue;
        }
        if (x < 0 && iv.x >= n-1.0){
            continue;
        }
        if ( x == 0 && (iv.x >= n - 0.01 || iv.x < -0.0)){
            continue;
        }

        vec2 cv = fv;
        cv.x += float(x) / n * n * oneScale;
        if (int(iv.x) - x == int(activeLight)){

            color += vec3(0.000,0.940,0.240) * vec3(ball(cv));
        }
        else {
            color += vec3(1.000,0.240,0.240) * vec3(ball(cv));
        }
    };


    fragColor = vec4(color, 1.0);
}
