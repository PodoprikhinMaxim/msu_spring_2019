#ifndef MATRIX_H
#define MATRIX_H

class Matrix
{
	private:
		int* matrix;
		std::size_t rows;
		std::size_t columns;
		std::size_t N;

		class Row
		{
			private:
				std::size_t len;
				int* row;
			public:
				//friend class Matrix;
				Row() : len(0) {}
				Row(std::size_t n, int* row_) : len(n), row(row_) {}
				/*~Row()
				{
					free(Row);
				}*/
				int &operator[](const std::size_t x)
				{
					if(x < 0 || x >= len)
					{
						throw std::out_of_range("");
					}
					return row[x];
				}
				const int &operator[](const std::size_t x) const
				{
					if(x < 0 || x >= len)
					{
						throw std::out_of_range("");
					}
					return row[x];
				}						
		};

	public:
		Matrix(std::size_t x, std::size_t y) : rows(x), columns(y), N(x * y)
		{
			matrix = new int[N];
		}
		~Matrix()
		{
			free(matrix);
		}
		Row operator[](const std::size_t x)
		{
			if(x < 0 || x >= rows)
			{
				throw std::out_of_range("");
			}
			return Row(columns, matrix + x*columns);
		}
		const Row operator[](const std::size_t x) const
		{
			if(x < 0 || x >= rows)
			{
				throw std::out_of_range("");
			}
			return Row(columns, matrix + x*columns);
		}
		std::size_t getRows() const
		{
			return rows;
		}
		std::size_t getColumns() const
		{
			return columns;
		}
		Matrix &operator*=(const int& a)
		{
			for(int i = 0; i < N; i++)
			{
				matrix[i] *= a;	
			}
			return *this;
		}
		bool operator==(const Matrix &B) const
		{
			if(columns != B.getColumns() || rows != B.getRows())
			{
				return false;
			}
			for (std::size_t i = 0; i < rows; i++) 
			{
				for (std::size_t j = 0; j < columns; j++) 
				{
					if ((*this)[i][j] != B[i][j])
					{
						return false;
					}
				}
			}
			return true;
		}
		bool operator!=(const Matrix &B) const 
		{
			return !(this == &B);
		}
};

#endif
