// Matriz Multiplicação Sequencial
// como compilar: gcc -O2 -o Sequencial Sequencial.c 
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
// Função para obter o tempo em segundos                        
double get_time_sec() { struct timeval tv; gettimeofday(&tv,NULL); return tv.tv_sec + tv.tv_usec*1e-6; }
// Função para alocar memória para uma matriz N x N
double *alloc_mat(long n) {
    double *m = malloc(sizeof(double)*n*n);
    if (!m) { perror("malloc"); exit(1); }
    return m;
}
// Função principal
int main(int argc, char **argv) {
    if (argc < 2) { printf("Usage: %s N\n", argv[0]); return 1; }
    long N = atol(argv[1]);
    double *A = alloc_mat(N);
    double *B = alloc_mat(N);
    double *C = alloc_mat(N);
    for (long i=0;i<N*N;i++){ A[i]=1.0; B[i]=1.0; C[i]=0.0; }
// Início da medição de tempo
    double t0 = get_time_sec();
    for (long i=0;i<N;i++){
        for (long k=0;k<N;k++){
            double aik = A[i*N + k];
            for (long j=0;j<N;j++){
                C[i*N + j] += aik * B[k*N + j];
            }
        }
    }
    // Fim da medição de tempo
    double t1 = get_time_sec();
    // Cálculo do checksum e impressão do tempo
    double s=0; for (long i=0;i<N*N;i++) s += C[i];
    printf("Checksum: %.3f\n", s);
    printf("Tempo: %.6f s\n", t1 - t0);
// Liberação de memória
    free(A); free(B); free(C);
    return 0;
}
