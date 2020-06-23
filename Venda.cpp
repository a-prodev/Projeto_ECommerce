#include "Venda.h"

namespace mrjp {

Venda::Venda():
    valorTotalDaCompra(0)
{

}

Venda::Venda(QString dataCompra)
{
    setDataCompra(dataCompra);

}

unsigned int Venda::getIdPedido() const
{
    return idPedido;
}

void Venda::setIdPedido(unsigned int value)
{
    idPedido = value;
}

unsigned int Venda::getIdCliente() const
{
    return idCliente;
}

void Venda::setIdCliente(unsigned int value)
{
    idCliente = value;
}

QString Venda::getDataCompra() const
{
    return dataCompra;
}

void Venda::setDataCompra(QString value)
{
    dataCompra = value;
}

jose::LDEC<Produto *> *Venda::getPListaDeProdutos() const
{
    return pListaDeProdutos;
}

void Venda::setPListaDeProdutos(jose::LDEC<Produto *> *value)
{
    pListaDeProdutos = value;
}

double Venda::getValorTotalDaCompra()
{
    return valorTotalDaCompra;
}

void Venda::setValorTotalDaCompra()
{
    double aux = 0;
    for(int i = 0; i < getPListaDeProdutos()->getQuantidade(); i++){
        aux += getPListaDeProdutos()->operator[](i + 1)->getPrecoTotal();
    }
    valorTotalDaCompra = aux;
}

QString Venda::print() const
{
    QString strPrint = QString::number(idPedido) + "\n" + QString::number(idCliente) + "\n" + getDataCompra() + "\n";

    for(int i = 0; i < getPListaDeProdutos()->getQuantidade(); i++)
        strPrint += getPListaDeProdutos()->operator[](i + 1)->print();

    return strPrint;
}

} // namespace mrjp
