# Gerenciador de Reserva de Salas

Este é o projeto final da disciplina **Estruturas de Dados II**, desenvolvido em C, que utiliza conceitos de **Árvore AVL**, **Hash** e **Heap**.
O objetivo principal do projeto é gerenciar a reserva de salas, permitindo a criação, listagem, reserva, bloqueio e consulta de salas de forma eficiente.

## Estruturas de Dados Utilizadas

- **Árvore AVL**: Utilizada para armazenar as salas, garantindo que a árvore permaneça balanceada após cada inserção ou remoção, o que otimiza a busca e a manutenção dos dados.
- **Hash**: Utilizado para armazenar e acessar rapidamente informações associadas às salas, como status de reserva.
- **Heap**: Utilizado para organizar e priorizar as operações, como a reserva de salas baseadas em sua capacidade.

## Melhorias Futuras

Embora o código já esteja funcionando para a maioria das operações básicas de manipulação de uma árvore AVL, há algumas melhorias que serão implementadas nas próximas versões para corrigir problemas e adicionar mais funcionalidades:

1. **Correção na remoção de nós na AVL**:
   - O código atual de remoção pode apresentar problemas de balanceamento em alguns casos específicos. Uma melhoria futura incluirá a revisão e correção do processo de remoção para garantir que a árvore AVL seja balanceada corretamente após a exclusão de nós.

2. **Implementação de funções faltantes**:
   - Algumas operações ainda não foram implementadas. Estas funcionalidades serão adicionadas para fornecer uma experiência mais completa no gerenciamento das salas. Cada uma dessas funções precisará de um controle de estado adicional para manipular as salas de forma eficiente.

3. **Otimização de operações de busca**:
   - Embora o código tenha uma busca recursiva simples para consultar salas, futuras melhorias podem incluir a otimização da busca e implementação de mecanismos mais rápidos ou eficientes, dependendo do tamanho da árvore.

Essas melhorias visam tornar o sistema mais robusto, eficiente e com uma experiência mais rica para os usuários.
