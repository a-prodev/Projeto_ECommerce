#include "crudprodutos.h"

namespace mrjp {

jose::LDEC<Produto *> *CRUDProdutos::getPEstoque() const
{
    return pEstoque;
}

void CRUDProdutos::setPEstoque(jose::LDEC<Produto *> *value)
{
    pEstoque = value;
}

CRUDProdutos::CRUDProdutos(QString nomeDoArquivoNoDisco):nomeDoArquivoNoDisco(nomeDoArquivoNoDisco)
{
   std::ifstream arquivo;
    arquivo.open(nomeDoArquivoNoDisco.toStdString().c_str());

    if(!arquivo.is_open()){
        std::ofstream novoArquivo(nomeDoArquivoNoDisco.toStdString().c_str());
        novoArquivo.close();
    }
    arquivo.close();

    try {
        pEstoque = new jose::LDEC<Produto *>;
    } catch (std::bad_alloc &) {
        throw QString("Erro: Falta de memória. Elemento não inserido.");
    }
}

void CRUDProdutos::criarLista()
{
    //abrir arquivo
    std::ifstream arquivo;
    arquivo.open(nomeDoArquivoNoDisco.toStdString().c_str(), std::ios::in);

    if(!arquivo.is_open())
        throw QString("Arquivo de produtos nao foi aberto");

    std::string linha;
    while(!arquivo.eof()){
        getline(arquivo,linha);
        getPEstoque()->inserirFim(montar(linha));
    }
    arquivo.close();
}

Produto *CRUDProdutos::montar(std::string linha)
{
    QStringList list = QString::fromStdString(linha).split(';');
    Produto * pProduto = new Produto(list[0].toUInt(),list[1],list[2].toUInt(),list[3].toFloat());
    return pProduto;
}

std::string CRUDProdutos::desmontar(QString print)
{
    QStringList list = print.split('\n');
    print = QString();
    for(int i = 0; i < list.size(); i++)
        print += list[i] + ";";
    print.chop(1); //nao incluir ultimo ';'
    return print.toStdString();
}

void CRUDProdutos::inserirNovoElemento(Produto *pProduto)
{
    std::ofstream arquivo;
    arquivo.open(nomeDoArquivoNoDisco.toStdString().c_str(), std::ios::out | std::ios::app);
    if(!arquivo.is_open())
        throw QString("Erro ao abrir arquivo de produtos - Metodo inserir");

    arquivo << desmontar(pProduto->print());
    arquivo.close();

    getPEstoque()->inserirFim(pProduto);
}

int CRUDProdutos::excluirElemento(unsigned int codProduto)
{
    for(int i = 0; i < pEstoque->getQuantidade(); i++)
        if(pEstoque->operator[](i + 1)->getCodigo() == codProduto)
        {
            std::ifstream arquivo;
            arquivo.open(nomeDoArquivoNoDisco.toStdString().c_str(), std::ios::in);

            if(!arquivo.is_open())
                throw QString("Arquivo nao foi aberto");

            std::ofstream arqTemp;
            arqTemp.open("temp.txt", std::ios::out);
            if(!arqTemp.is_open()){
                throw QString("Arquivo temporario nao foi aberto");
            }
            std::string linha;

            while(getline(arquivo, linha))
                if(QString::fromStdString(linha) == pEstoque->operator[](i + 1)->print())
                    continue;
                else
                    arqTemp << linha + "\n";
            //fecha os arquivos
            arquivo.close();
            arqTemp.close();

            remove(nomeDoArquivoNoDisco.toStdString().c_str());
            rename("temp.txt", nomeDoArquivoNoDisco.toStdString().c_str());

            pEstoque->retirarPosicao(i + 1);
            return 0;
        }
    return -1;
}

unsigned int CRUDProdutos::gerarID()
{
    unsigned int i = 0;
    for(; i < getPEstoque()->getQuantidade(); i++)
        if(getPEstoque()->operator[](i + 1)->getCodigo() != i)
            return i;
    return i;
}

} // namespace mrjp
