#include "AIPlayer.h"
#include "Parchis.h"

const double masinf = 9999999999.0, menosinf = -9999999999.0;
const double gana = masinf - 1, pierde = menosinf + 1;
const int num_pieces = 3;
const int PROFUNDIDAD_MINIMAX = 4;  // Umbral maximo de profundidad para el metodo MiniMax
const int PROFUNDIDAD_ALFABETA = 6; // Umbral maximo de profundidad para la poda Alfa_Beta

bool AIPlayer::move()
{
    cout << "Realizo un movimiento automatico" << endl;

    color c_piece;
    int id_piece;
    int dice;
    think(c_piece, id_piece, dice);

    cout << "Movimiento elegido: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    actual->movePiece(c_piece, id_piece, dice);
    return true;
}

void AIPlayer::think(color &c_piece, int &id_piece, int &dice) const
{

    /*     switch (id)
        {
        case 0:
            thinkAleatorio(c_piece, id_piece, dice);
            break;
        case 1:
            thinkAleatorioMasInteligente(c_piece, id_piece, dice);
            break;
            case 2:
                thinkFichaMasAdelantada(c_piece, id_piece, dice);
                break;
            case 3:
                thinkMejorOpcion(c_piece, id_piece, dice);
                break;
        } */

    // El siguiente código se proporciona como sugerencia para iniciar la implementación del agente.

    double valor;                           // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha = menosinf, beta = masinf; // Cotas iniciales de la poda AlfaBeta
    // Llamada a la función para la poda (los parámetros son solo una sugerencia, se pueden modificar).
    valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
    cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    // ----------------------------------------------------------------- //

    // Si quiero poder manejar varias heurísticas, puedo usar la variable id del agente para usar una u otra.
    switch (id)
    {
    case 0:
        valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
        break;
    case 1:
        valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion1);
        break;
        /* case 2:
            valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion2);
            break; */
    }
    cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;
}

double AIPlayer::ValoracionTest(const Parchis &estado, int jugador)
{
    // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.

    int ganador = estado.getWinner();
    int oponente = (jugador + 1) % 2;

    // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
    if (ganador == jugador)
    {
        return gana;
    }
    else if (ganador == oponente)
    {
        return pierde;
    }
    else
    {
        // Colores que juega mi jugador y colores del oponente
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        // Recorro todas las fichas de mi jugador
        int puntuacion_jugador = 0;
        // Recorro colores de mi jugador.
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                // Valoro positivamente que la ficha esté en casilla segura o meta.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_jugador++;
                }
                else if (estado.getBoard().getPiece(c, j).get_box().type == goal)
                {
                    puntuacion_jugador += 5;
                }
            }
        }

        // Recorro todas las fichas del oponente
        int puntuacion_oponente = 0;
        // Recorro colores del oponente.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                if (estado.isSafePiece(c, j))
                {
                    // Valoro negativamente que la ficha esté en casilla segura o meta.
                    puntuacion_oponente++;
                }
                else if (estado.getBoard().getPiece(c, j).get_box().type == goal)
                {
                    puntuacion_oponente += 5;
                }
            }
        }

        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        return puntuacion_jugador - puntuacion_oponente;
    }
}

void AIPlayer::thinkAleatorio(color &c_piece, int &id_piece, int &dice) const
{
    // IMPLEMENTACIÓN INICIAL DEL AGENTE
    // Esta implementación realiza un movimiento aleatorio.
    // Se proporciona como ejemplo, pero se debe cambiar por una que realice un movimiento inteligente
    // como lo que se muestran al final de la función.

    // OBJETIVO: Asignar a las variables c_piece, id_piece, dice (pasadas por referencia) los valores,
    // respectivamente, de:
    // - color de ficha a mover
    // - identificador de la ficha que se va a mover
    // - valor del dado con el que se va a mover la ficha.

    // El id de mi jugador actual.
    int player = actual->getCurrentPlayerId();

    // Vector que almacenará los dados que se pueden usar para el movimiento
    vector<int> current_dices;
    // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido.
    vector<tuple<color, int>> current_pieces;

    // Se obtiene el vector de dados que se pueden usar para el movimiento
    current_dices = actual->getAvailableNormalDices(player);
    // Elijo un dado de forma aleatoria.
    dice = current_dices[rand() % current_dices.size()];

    // Se obtiene el vector de fichas que se pueden mover para el dado elegido
    current_pieces = actual->getAvailablePieces(player, dice);

    // Si tengo fichas para el dado elegido muevo una al azar.
    if (current_pieces.size() > 0)
    {
        int random_id = rand() % current_pieces.size();
        id_piece = get<1>(current_pieces[random_id]); // get<i>(tuple<...>) me devuelve el i-ésimo
        c_piece = get<0>(current_pieces[random_id]);  // elemento de la tupla
    }
    else
    {
        // Si no tengo fichas para el dado elegido, pasa turno (la macro SKIP_TURN me permite no mover).
        id_piece = SKIP_TURN;
        c_piece = actual->getCurrentColor(); // Le tengo que indicar mi color actual al pasar turno.
    }
}

void AIPlayer::thinkAleatorioMasInteligente(color &c_piece, int &id_piece, int &dice) const
{
    // El número de mi jugador actual.
    int player = actual->getCurrentPlayerId();
    // Vector que almacenará los dados que se pueden usar para el movimiento.
    vector<int> current_dices_con_especiales;
    // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido.
    vector<tuple<color, int>> current_pieces;
    // Obtengo el vector de dados que puedo usar para el movimiento.
    // En este caso elijo todos, tanto normales como especiales.
    // Importante: puedo acceder a solo dados normales o especiales por separado,
    // o a todos a la vez:
    // - actual->getAvailableNormalDices(player) -> solo dados normales
    // - actual->getAvailableSpecialDices(player) -> solo dados especiales
    // - actual->getAllAvailableDices(player) -> todos los dados
    // Importante 2: los "available" me dan los dados que puedo usar en el turno actual.
    // Por ejemplo, si me tengo que contar 10 o 20 solo me saldrán los dados 10 y 20.
    // Puedo saber qué más dados tengo, aunque no los pueda usar en este turno, con:
    // - actual->getNormalDices(player) -> todos los dados normales
    // - actual->getSpecialDices(player) -> todos los dados especiales
    // - actual->getAllDices(player) -> todos los dados
    current_dices_con_especiales = actual->getAllAvailableDices(player);
    // En vez de elegir un dado al azar, miro primero cuáles tienen fichas que se puedan mover.
    vector<int> current_dices_que_pueden_mover_ficha;
    for (int i = 0; i < current_dices_con_especiales.size(); i++)
    {
        // Se obtiene el vector de fichas que se pueden mover para el dado elegido.
        current_pieces = actual->getAvailablePieces(player, current_dices_con_especiales[i]);
        // Si se pueden mover fichas para el dado actual, lo añado al vector de dados que pueden mover fichas.
        if (current_pieces.size() > 0)
        {
            current_dices_que_pueden_mover_ficha.push_back(current_dices_con_especiales[i]);
        }
    }
    // Si no tengo ninún dado que pueda mover fichas, paso turno con un dado al azar (la macro SKIP_TURN me
    if (current_dices_que_pueden_mover_ficha.size() == 0)
    {
        dice = current_dices_con_especiales[rand() % current_dices_con_especiales.size()];
        id_piece = SKIP_TURN;
        c_piece = actual->getCurrentColor(); // Le tengo que indicar mi color actual al pasar turno.
    }
    // En caso contrario, elijo un dado de forma aleatoria de entre los que pueden mover ficha.
    else
    {
        dice = current_dices_que_pueden_mover_ficha[rand() % current_dices_que_pueden_mover_ficha.size()];
        // Se obtiene el vector de fichas que se pueden mover para el dado elegido.
        current_pieces = actual->getAvailablePieces(player, dice);
        // Muevo una ficha al azar de entre las que se pueden mover.
        int random_id = rand() % current_pieces.size();
        id_piece = get<1>(current_pieces[random_id]);
        c_piece = get<0>(current_pieces[random_id]);
    }
}

double AIPlayer::Poda_AlfaBeta(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, double alpha, double beta, double (*heuristic)(const Parchis &, int)) const
{
    color ultimo_color = c_piece;
    int ultimo_id_piece = id_piece;
    int ultimo_dice = dice;

    bool sigue = true;
    int alpha_aux, beta_aux;

    if (profundidad == profundidad_max || actual.gameOver())
    {
        return heuristic(actual, jugador);
    }

    if (jugador == actual.getCurrentPlayerId())
    { // MAX
        ParchisBros hijos = actual.getChildren();
        for (ParchisBros::Iterator it = hijos.begin(); it != hijos.end() && sigue; ++it)
        {

            alpha_aux = Poda_AlfaBeta(*it, jugador, profundidad + 1, profundidad_max, ultimo_color, ultimo_id_piece, ultimo_dice, alpha, beta, heuristic);

            if (alpha_aux > alpha)
            {
                alpha = alpha_aux;
                if (profundidad == 0)
                {
                    ultimo_color = it.getMovedColor();
                    ultimo_id_piece = it.getMovedPieceId();
                    ultimo_dice = it.getMovedDiceValue();
                }
            }

            if (beta <= alpha)
            {
                sigue = false;
            }
        }
        c_piece = ultimo_color;
        id_piece = ultimo_id_piece;
        dice = ultimo_dice;
        return alpha;
    }
    else
    { // MIN
        ParchisBros hijos = actual.getChildren();
        for (ParchisBros::Iterator it = hijos.begin(); it != hijos.end() && sigue; ++it)
        {

            beta_aux = Poda_AlfaBeta(*it, jugador, profundidad + 1, profundidad_max, ultimo_color, ultimo_id_piece, ultimo_dice, alpha, beta, heuristic);

            if (beta_aux < beta)
            {
                beta = beta_aux;
                if (profundidad == 0)
                {
                    ultimo_color = it.getMovedColor();
                    ultimo_id_piece = it.getMovedPieceId();
                    ultimo_dice = it.getMovedDiceValue();
                }
            }

            if (beta <= alpha)
            {
                sigue = false;
            }
        }
        c_piece = ultimo_color;
        id_piece = ultimo_id_piece;
        dice = ultimo_dice;
        return beta;
    }
}

double AIPlayer::MiValoracion1(const Parchis &estado, int jugador)
{
    int ganador = estado.getWinner();
    int oponente = (jugador + 1) % 2;

    // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
    if (ganador == jugador)
    {
        return gana;
    }
    else if (ganador == oponente)
    {
        return pierde;
    }
    else
    {
        // Colores que juega mi jugador y colores del oponente
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        // Recorro todas las fichas de mi jugador
        int puntuacion_jugador = 0;
        // Recorro colores de mi jugador.
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                // Valoro positivamente que la ficha esté en casilla segura o meta.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_jugador += 35;
                }
                else if (estado.getBoard().getPiece(c, j).get_box().type == goal)
                {
                    puntuacion_jugador += 50;
                }
                else if (estado.eatenPiece().first != my_colors[(i + 1) % 2])
                {
                    puntuacion_jugador += 30;
                }
                else if (estado.distanceToGoal(c, j) < 13 && estado.isSafePiece(c, j))
                {
                    puntuacion_jugador += 60;
                }
                if (estado.distanceToGoal(c, j) < 50)
                {
                    puntuacion_jugador += 20;
                }
                if (estado.distanceToGoal(c, j) < 40)
                {
                    puntuacion_jugador += 25;
                }
                if (estado.distanceToGoal(c, j) < 30)
                {
                    puntuacion_jugador += 30;
                }
                if (estado.overBounce())
                {
                    puntuacion_jugador -= 20;
                }
                if (estado.isWall(estado.getBoard().getPiece(c, j).get_box()))
                {
                    puntuacion_jugador += 15;
                }
                if (oponenteDetras(estado, my_colors, op_colors))
                {
                    puntuacion_jugador -= 30;
                }
                if (estado.eatenPiece().first != op_colors[0] and estado.eatenPiece().first != op_colors[1] and estado.eatenPiece().first != none)
                {
                    puntuacion_jugador += 17; // si he comido alguna ficha del oponente sumo 17
                }
                if (estado.eatenPiece().first == op_colors[0] or estado.eatenPiece().first == op_colors[1])
                {
                    puntuacion_jugador += 50; // si he comido alguna ficha mia resto 50
                }
                if (!estado.isSafePiece(c, j) and estado.distanceToGoal(c, j) < 9 and oponenteDetras(estado, my_colors, op_colors))
                {
                    puntuacion_jugador -= 20; // si detrás de una ficha mia hay una que me pueda comer resto 20
                }
            }
        }

        // Recorro todas las fichas del oponente
        int puntuacion_oponente = 0;
        // Recorro colores del oponente.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_oponente += 35;
                }
                else if (estado.getBoard().getPiece(c, j).get_box().type == goal)
                {
                    puntuacion_oponente += 50;
                }
                else if (estado.eatenPiece().first != my_colors[(i + 1) % 2])
                {
                    puntuacion_oponente += 30;
                }
                else if (estado.distanceToGoal(c, j) < 13 && estado.isSafePiece(c, j))
                {
                    puntuacion_oponente += 60;
                }
                if (estado.distanceToGoal(c, j) < 50)
                {
                    puntuacion_oponente += 20;
                }
                if (estado.distanceToGoal(c, j) < 40)
                {
                    puntuacion_oponente += 25;
                }
                if (estado.distanceToGoal(c, j) < 30)
                {
                    puntuacion_oponente += 30;
                }
                if (estado.overBounce())
                {
                    puntuacion_oponente -= 20;
                }
                if (estado.isWall(estado.getBoard().getPiece(c, j).get_box()))
                {
                    puntuacion_oponente += 15;
                }
                if (oponenteDetras(estado, my_colors, op_colors))
                {
                    puntuacion_oponente -= 30;
                }
                if (estado.eatenPiece().first != op_colors[0] and estado.eatenPiece().first != op_colors[1] and estado.eatenPiece().first != none)
                {
                    puntuacion_oponente += 17; // si he comido alguna ficha del oponente sumo 17
                }
                if (estado.eatenPiece().first == op_colors[0] or estado.eatenPiece().first == op_colors[1])
                {
                    puntuacion_oponente += 50; // si he comido alguna ficha mia resto 50
                }
                if (!estado.isSafePiece(c, j) and estado.distanceToGoal(c, j) < 9 and oponenteDetras(estado, my_colors, op_colors))
                {
                    puntuacion_oponente -= 20; // si detrás de una ficha mia hay una que me pueda comer resto 20
                }
            }
        }

        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        return puntuacion_jugador - puntuacion_oponente;
    }
}

bool AIPlayer::oponenteDetras(const Parchis &estado, vector<color> mis_colores, vector<color> colores_oponente)
{

    bool detras = false;

    for (int i = 0; i < mis_colores.size() && !detras; i++)
    {
        for (int j = 0; j < num_pieces && !detras; j++)
        {
            color c = mis_colores[i];
            int mi_posicion = estado.getBoard().getPiece(c, j).get_box().num;

            for (int k = 0; k < colores_oponente.size() && !detras; k++)
            {
                for (int l = 0; l < num_pieces && !detras; l++)
                {
                    color c2 = colores_oponente[k];
                    int op_posicion = estado.getBoard().getPiece(c2, l).get_box().num;

                    if ((mi_posicion - op_posicion) <= 6)
                        detras = true;
                }
            }
        }
    }

    return detras;
}