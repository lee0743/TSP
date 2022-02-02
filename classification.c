#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define TRUE (1)
#define FALSE (0)

enum {
	LINE_LENGTH = 1024,
	NUM_SAMPLES = 150,
	NUM_FEATURES = 4,
	NUM_ANSWER = 3,
};

enum {
	ANS_SETOSA = 0,
	ANS_VERSICOLOR = 1,
	ANS_VIRGINICA = 2
};

double g_weights[NUM_ANSWER][NUM_FEATURES];
double g_samples[NUM_SAMPLES][NUM_FEATURES];
double g_predicts[NUM_SAMPLES][NUM_ANSWER];
double g_answers[NUM_SAMPLES][NUM_ANSWER];

double g_lose;

void initialize_weights(void);

int main(void) 
{
	FILE* fstream = fopen("irisdataset.csv", "r");
	assert(fstream != NULL);
	
	char lines[LINE_LENGTH];
	fgets(lines, LINE_LENGTH, fstream);	// 첫째 줄 무시
	
	for (size_t i = 0; i < NUM_SAMPLES; ++i) {
		char answer[LINE_LENGTH];
		fgets(lines, LINE_LENGTH, fstream);
		
		sscanf(lines, "%lf,%lf,%lf,%lf,%s",
			   &g_samples[i][0], 
			   &g_samples[i][1], 
			   &g_samples[i][2], 
			   &g_samples[i][3], 
			   answer);
		
		if (0 == strcmp(answer, "setosa")) {
			g_answers[i][ANS_SETOSA] = 1;
		} else if (0 == strcmp(answer, "versicolor")) {
			g_answers[i][ANS_VERSICOLOR] = 1;
		} else if (0 == strcmp(answer, "virginica")) {
			g_answers[i][ANS_VIRGINICA] = 1;
		} else {
			assert(FALSE);
		}
	}
	
	fclose(fstream);
	
	initialize_weights();
	
	size_t count = 10;
	const double GROW_RATE = 0.01;
	
	while (count-- > 0) {
		for (size_t i = 0; i < NUM_SAMPLES; ++i) {
			double dotProducts[NUM_ANSWER] = {0, };
			double dotProductSum = 0.0;

			for (size_t j = 0; j < NUM_ANSWER; ++j) {
				for (size_t k = 0; k < NUM_FEATURES; ++k) {
					dotProducts[j] += (g_samples[i][k] * g_weights[k][j]); 
				}

				dotProductSum += dotProducts[j];
			}

			// 소프트맥스 함수
			for (size_t j = 0; j < NUM_ANSWER; ++j) {
				g_predicts[i][j] = exp(dotProducts[j]) / exp(dotProductSum);
				assert (0 < g_predicts[i][j] < 1);
			}
		}

		g_lose = 0.0;
		for (size_t i = 0; i < NUM_SAMPLES; ++i) {
			for (size_t j = 0; j < NUM_ANSWER; ++j) {
				assert(0 < g_predicts[i][j] <= 1);
				g_lose += (g_answers[i][j] * log(g_predicts[i][j])); 
			}
		}
		g_lose /= (-1.0 / NUM_SAMPLES);
		printf("%lf\n", g_lose);	
		
		for (size_t i = 0; i < NUM_FEATURES; ++i) {
			for (size_t j = 0; j < NUM_ANSWER; ++j) {
				double derivative = 0.0;
		
				for (size_t k = 0; k < NUM_SAMPLES; ++k) {
					double diff = g_answers[k][j] - g_predicts[k][j];
					derivative += diff * g_samples[k][i];
				}
				derivative /= NUM_SAMPLES;
				g_weights[j][i] -= (derivative * GROW_RATE);
			}
		}
	}
	
	return 0;
}

void initialize_weights(void) 
{
	srand(time(NULL));

	for (size_t i = 0; i < NUM_ANSWER; ++i) {
		for (size_t j = 0; j < NUM_FEATURES; ++j) {
			g_weights[i][j] = rand() % 10000 / 10000.0;
		}
	}
}

// main: main.c:86: int main(void): Assertion `0 < g_predicts[i][j] < 1' failed.
main: main.c:86: int main(void): Assertion `0 < g_predicts[i][j] < 1' failed. 왜 버그를 못찾았을까???
