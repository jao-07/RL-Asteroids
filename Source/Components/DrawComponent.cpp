//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "DrawComponent.h"
#include "CircleColliderComponent.h"
#include "../Game.h"

DrawComponent::DrawComponent(class Actor* owner, std::vector<Vector2> &vertices, int drawOrder)
    :Component(owner)
    ,mVertices(vertices)
    ,mDrawOrder(drawOrder)
{
    mOwner->GetGame()->AddDrawable(this);
}

DrawComponent::~DrawComponent()
{
    mOwner->GetGame()->RemoveDrawable(this);
}

void DrawComponent::DrawPolygon(SDL_Renderer *renderer, std::vector<Vector2>& vertices)
{
    Vector2 pos = mOwner->GetPosition();

    // --------------
    // TODO - PARTE 1
    // --------------

    // TODO 1.1 (~3 linhas): percorra do primeiro até o penúltimo vértices, utilizando a função
    //  SDL_RenderDrawLine para desenhar linhas entre os vértices i e i+1. Some a posição do dono
    //  do componente (pos) a cada vértice.


    // TODO 1.2 (~1 linha): Utilize a função SDL_RenderDrawLine para desenhar uma linha entre o último
    //  e o primeiro vértice. Some a posição do dono do componente (pos) a cada vértice.

}

void DrawComponent::DrawCircle(SDL_Renderer *renderer, const Vector2& center, const float radius, const int numVertices)
{
    std::vector<Vector2> vertices;

    // --------------
    // TODO - PARTE 1
    // --------------

    // TODO 2.1 (~1 linha): inicialize variável angle (float) com zero. Ela será
    //  utilizada para percorrer o arco de uma circunferência em intervalos angulares de tamanho fixo.

    // TODO 2.2 (~6 linhas): Repita o seguinte procedimento para um dado número de vértices (numVertices):
    //  (a) Calcule a coordenada x do novo vértice multiplicando o raio da circunferência (radius) pelo cosseno
    //  do ângulo corrente (angle);
    //  (b) Calcule a coordenada y da mesma forma, porém multiplicando pelo seno do ângulo corrente;
    //  (c) Some o vetor (x,y) com o centro (center) e adicione o vetor resultante ao conjunto de vértices (vertices);
    //  (d) Incremente o ângulo corrente por 2*PI dividido pelo número de vértices (numVertices).

    DrawComponent::DrawPolygon(renderer, vertices);
}

void DrawComponent::Draw(SDL_Renderer *renderer)
{
    // --------------
    // TODO - PARTE 1
    // --------------

    // TODO 3.1 (~1 linha): Utilize a função Matrix3::CreateRotation para criar uma matriz de rotação com o ângulo
    //  do dono desse componente (-mOwner->GetRotation()).

    // TODO 3.2 (~4 linhas): Percorra os vértices desse componente (mVertices), multiplicando-os pela matriz de
    //  rotação com a função Vector2::Transform. Adicione o vetor transformado à um vetor temporária de vertices.

    // TODO 3.3 (~1 linha): Utilize a função SDL_SetRenderDrawColor para alterar a cor de desenho para branco.

    // TODO 3.4 (~1 linha): Chame a função DrawPolygon para desenhar o conjunto de vértices transformado.

    // TODO 3.5 (~4 linhas): Utilize a função DrawCircle para desenhar o círculo de colisão desse objeto.
    //  Antes de desenhar, altere a cor para verde com a função SDL_SetRenderDrawColor. Esse trecho
    //  de código será útil para debugar a detecção de colisão.

}