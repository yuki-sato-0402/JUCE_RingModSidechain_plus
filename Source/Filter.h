#include <JuceHeader.h>
class OnePoleLowPass
{
public:
    void setAlpha(float newAlpha)
    {
        alpha = newAlpha;
    }

    // y[n] = (1 - α) * x[n] + α * y[n-1]
    float processSample(float input)
    {
        float output = (1.0f - alpha) * input + alpha * z1;
        z1 = output;
        return output;
    }

    void reset()
    {
        z1 = 0.0f;
    }

private:
    float alpha = 0.0f;  
    float z1 = 0.0f;     // Retention of y[n-1
};
