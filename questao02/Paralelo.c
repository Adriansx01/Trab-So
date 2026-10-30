//Matriz Multiplicação Paralela usando Pthreads
// como compilar: gcc -O2 -pthread -o Paralelo Paralelo.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

// Estrutura para passar argumentos para as threads
typedef struct {
    int tid;
    int P;
    long N;
    double *A,*B,*C;
    long rstart, rend;
} mat_arg_t;

double get_time_sec() { struct timeval tv; gettimeofday(&tv,NULL); return tv.tv_sec + tv.tv_usec*1e-6; }
// Função executada por cada thread
void *worker(void *arg) {
    mat_arg_t *m = (mat_arg_t*)arg;
    long N = m->N;
    for (long i = m->rstart; i < m->rend; ++i) {
        for (long k = 0; k < N; ++k) {
            double aik = m->A[i*N + k];
            for (long j = 0; j < N; ++j) {
                m->C[i*N + j] += aik * m->B[k*N + j];
            }
        }
    }
    return NULL;
}
// Função para alocar memória para uma matriz N x N
double *alloc_mat(long n) {
    double *m = malloc(sizeof(double)*n*n);
    if (!m) { perror("malloc"); exit(1); }
    return m;
}
// Função principal
int main(int argc, char **argv) {
    if (argc < 3) { printf("Usage: %s N P\n", argv[0]); return 1; }
    long N = atol(argv[1]);
    int P = atoi(argv[2]);
    double *A = alloc_mat(N), *B = alloc_mat(N), *C = alloc_mat(N);
    for (long i=0;i<N*N;i++){ A[i]=1.0; B[i]=1.0; C[i]=0.0; }

    pthread_t *ths = malloc(sizeof(pthread_t)*P);
    mat_arg_t *args = malloc(sizeof(mat_arg_t)*P);
// Divisão do trabalho entre as threads
    long base = N / P; long rem = N % P; long offset = 0;
    for (int p=0;p<P;p++){
        long chunk = base + (p < rem ? 1 : 0);
        args[p].tid = p; args[p].P = P; args[p].N = N;
        args[p].A = A; args[p].B = B; args[p].C = C;
        args[p].rstart = offset; args[p].rend = offset + chunk;
        offset += chunk;
    }
// Início da medição de tempo
    double t0 = get_time_sec();
    for (int p=0;p<P;p++) pthread_create(&ths[p], NULL, worker, &args[p]);
    for (int p=0;p<P;p++) pthread_join(ths[p], NULL);
    double t1 = get_time_sec();

    double s=0; for (long i=0;i<N*N;i++) s+=C[i];
    printf("Checksum: %.3f\n", s);
    printf("Threads: %d  Tempo: %.6f s\n", P, t1 - t0);
// Liberação de memória
    free(A); free(B); free(C); free(ths); free(args);
    return 0;
}
