# Relatório de Resultados - Fila de Prioridade com Repriorização Dinâmica

**Nome:** Daniel Trindade , Yago Lobato  
**Disciplina:** Laboratório de Programação Avançada  
**Professor:** César Melo  
**Data:** 30/05/2025

## 1. Introdução

### 1.1 Objetivo
Este relatório apresenta os resultados dos testes realizados na implementação de uma fila de prioridade concorrente com capacidade de repriorização dinâmica, conforme especificado no enunciado do trabalho.

### 1.2 Implementação
A solução utiliza:
- **Estrutura de dados**: Max-heap implementado com array
- **Sincronização**: pthread_mutex para thread-safety
- **Operações**: inserir(), remover(), repriorizar()
- **Capacidade**: 100 elementos
- **Tipo de dado**: Pedidos de leitura de sensor de temperatura

### 1.3 Metodologia de Teste
Foram realizados três casos de teste conforme especificado:
- **Caso 1**: Variação do número de produtores (alta concorrência de inserção)
- **Caso 2**: Variação da frequência de repriorização (integridade sob atualizações)
- **Caso 3**: Variação do número de consumidores (consumo intenso)

## 2. Desenvolvimento dos Casos de Teste

### 2.1 Configuração Base
- **Produtores**: 2 threads (padrão)
- **Consumidores**: 2 threads (padrão)  
- **Repriorizadores**: 1 thread (padrão)
- **Operações por thread**: 50 inserções, 50 remoções, 20 repriorizações

---

## 3. Resultados

### 3.1 Caso 1: Alta Concorrência de Inserção
**Variável testada**: Número de produtores  
**Importância**: Testar inserções simultâneas

#### Configuração 1A - 1 Produtor (baseline)
```
Configuração: 1 produtor, 2 consumidores, 1 repriorizador
```
- **Tempo de execução**: 10 segundos
- **Número de inserções**: 50
- **Número de remoções**: 50
- **Repriorizações**: 20 (0 bem-sucedidas)
- **Taxa de sucesso**: 0%
- **Tamanho final da fila:** 0

#### Configuração 1B - 2 Produtores (padrão)
```
Configuração: 2 produtores, 2 consumidores, 1 repriorizador
```
- **Tempo de execução**: 10 segundos
- **Número de inserções**: 100 
- **Número de remoções**: 100
- **Repriorizações**: 20 (7 bem-sucedidas)
- **Taxa de sucesso**: 35%
- **Tamanho final da fila:** 0

#### Configuração 1C - 4 Produtores (alta concorrência)
```
Configuração: 4 produtores, 2 consumidores, 1 repriorizador
```
- **Tempo de execução**: 10 segundos
- **Número de inserções**: 150 
- **Número de remoções**: 100
- **Repriorizações**: 20 (10 bem-sucedidas)
- **Taxa de sucesso**: 50%
- **Tamanho final da fila:** 50

#### Análise do Caso 1  
**Comportamento observado**:  
Observamos que aumentando o número de produtores, pudemos observar um crescimento proporcional no número de inserções, demonstrando que a paralelização funcionou. E também observa-se um aumento na taxa de sucesso das repriorizações, indicando que uma fila mais populosa oferece mais oportunidade para encontrar elementos para reprioritizar.

**Conclusões sobre concorrência de inserção**:  
A implementação demonstrou capacidade para conseguir lidar com vários produtores simultâneos. Com um crescimento linear das inserções, infere-se uma baixa contenção do mutex. Na configuração 1C, com um acúmulo de elementos na fila indica que 4 produtores superam a capacidade de 2 consumidores, explicitando a necessidade de encontrarmos um equilibrio entre produção e consumo

---

### 3.2 Caso 2: Alta Frequência de Repriorização
**Variável testada**: Frequência de chamadas de repriorização  
**Importância**: Testar integridade sob atualizações concorrentes

#### Configuração 2A - Baixa Frequência (500ms)
```
Configuração: 2 produtores, 2 consumidores, 1 repriorizador (500ms)
```
- **Tempo de execução**: 10 segundos
- **Número de inserções**: 100 
- **Número de remoções**: 100
- **Repriorizações**: 20 (5 bem-sucedidas)
- **Taxa de sucesso**: 25%
- **Tamanho final da fila:** 0

#### Configuração 2B - Média Frequência (200ms)
```
Configuração: 2 produtores, 2 consumidores, 1 repriorizador (200ms)
```
- **Tempo de execução**: 10 segundos
- **Número de inserções**: 100 
- **Número de remoções**: 99
- **Repriorizações**: 20 (2 bem-sucedidas)
- **Taxa de sucesso**: 10%
- **Tamanho final da fila:** 1

#### Configuração 2C - Alta Frequência (50ms)
```
Configuração: 2 produtores, 2 consumidores, 1 repriorizador (50ms)
```
- **Tempo de execução**: 10 segundos
- **Número de inserções**: 100 
- **Número de remoções**: 99
- **Repriorizações**: 20 (0 bem-sucedidas)
- **Taxa de sucesso**: 0%
- **Tamanho final da fila:** 1

#### Análise do Caso 2
**Comportamento observado**:
Aqui observa-se uma relação inversa entre frequência de reprioritização e taxa de sucesso, evidente pela queda da taxa. Isso sugere que reprioritização mais frequentes competem em igualdade com inserção e remoção pelo acesso ao mutex, reduzindo as chances de encontrar elementos válidos na fila.

**Conclusões sobre frequência de repriorização**:
Com a alta frequência de repriorização a eficácia da operação fica significantemente prejudicada. Isso acontece pelo fato de a busca linear por lementos específicos na fila ser custosa, e com alta concorrência. Assim a implementação funciona melhor com reprioritizações menos frequentes, sugerindo que a operação de reprioritização deve ser usada de maneira estratégica.

---

### 3.3 Caso 3: Consumo Intenso
**Variável testada**: Número de consumidores  
**Importância**: Testar desempenho sob carga de remoção

#### Configuração 3A - 1 Consumidor (baseline)
```
Configuração: 2 produtores, 1 consumidor, 1 repriorizador
```
- **Tempo de execução**: 10 segundos
- **Número de inserções**: 100 
- **Número de remoções**: 50
- **Repriorizações**: 20 (7 bem-sucedidas)
- **Taxa de sucesso**: 35%
- **Tamanho final da fila**: 50

#### Configuração 3B - 2 Consumidores (padrão)
```
Configuração: 2 produtores, 2 consumidores, 1 repriorizador
```
- **Tempo de execução**: 10 segundos
- **Número de inserções**: 100 
- **Número de remoções**: 99
- **Repriorizações**: 20 (5 bem-sucedidas)
- **Taxa de sucesso**: 25%
- **Tamanho final da fila**: 1

#### Configuração 3C - 4 Consumidores (consumo intenso)
```
Configuração: 2 produtores, 4 consumidores, 1 repriorizador
```
- **Tempo de execução**: 10 segundos
- **Número de inserções**: 100 
- **Número de remoções**: 100
- **Repriorizações**: 20 (0 bem-sucedidas)
- **Taxa de sucesso**: 0%
- **Tamanho final da fila**: 0

#### Análise do Caso 3
**Comportamento observado**:
O aumento de consumidores melhorou muito a capacidade de processamento, demonstrando eficácia da paralelização no consumo. Entretanto, uma fila mais vazia resultou em menor taxa de sucesso das repriorizações, já que há menos elementos disponíveis para localizar e repriorizar.

**Conclusões sobre consumo intenso**:
Múltiplos consumidores são altamente eficazes para drenar a fila rapidamente, praticamente eliminando filas de espera. Porém, isso cria um trade-off: filas vazias reduzem as oportunidades de repriorização. Para aplicações que dependem de repriorização frequente, é necessário balancear cuidadosamente o número de consumidores para manter elementos suficientes na fila.

---

## 4. Análise Comparativa

### 4.1 Tabela Resumo dos Resultados

| Caso | Configuração | Tempo(s) | Inserções | Remoções | Repriorizações | Taxa Sucesso(%) |
|------|-------------|----------|-----------|----------|----------------|-----------------|
| 1A   | 1P-2C-1R   | 10       | 50       | 50      | 20 (0)        | 0%             |
| 1B   | 2P-2C-1R   | 10       | 100      | 100     | 20 (7)        | 35%            |
| 1C   | 4P-2C-1R   | 10       | 150      | 100     | 20 (10)       | 50%            |
| 2A   | 2P-2C-1R(500ms) | 10   | 100      | 100     | 20 (5)        | 25%            |
| 2B   | 2P-2C-1R(200ms) | 10   | 100      | 99      | 20 (2)        | 10%            |
| 2C   | 2P-2C-1R(50ms)  | 10   | 100      | 99      | 20 (0)        | 0%             |
| 3A   | 2P-1C-1R   | 10       | 100      | 50      | 20 (7)        | 35%            |
| 3B   | 2P-2C-1R   | 10       | 100      | 99      | 20 (5)        | 25%            |
| 3C   | 2P-4C-1R   | 10       | 100      | 100     | 20 (0)        | 0%             |

### 4.2 Análise de Desempenho
**Melhor configuração para inserções**:
Configuração 1C (4 produtores) alcançou 150 inserções, demonstrando que múltiplos produtores escalam bem até o limite testado.

**Melhor configuração para remoções**:
Configurações 1B, 2A e 3C conseguiram processar completamente (99-100 remoções), com 3C sendo mais eficiente por drenar a fila completamente.

**Configuração mais equilibrada**:
Configuração 1B (2P-2C-1R) oferece o melhor equilíbrio entre throughput (100 inserções/100 remoções) e funcionalidade de repriorização (35% de sucesso).

## 5. Conclusão
Os resultados indicam que o design de sistemas concorrentes requer cuidadoso balanceamento entre diferentes objetivos de performance. Para aplicações reais, recomenda-se ajustar os parâmetros de concorrência baseado no perfil específico de uso, priorizando ou throughput máximo ou funcionalidades avançadas conforme necessário.