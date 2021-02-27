#include "gtest/gtest.h"
#include "ta_abstract.h"
#include "ta_func.h"

TEST(talib, func_ma)
{
  uint size = 10;
  TA_Real closePrice[size];
  TA_Real out[size];
  TA_Integer outBeg;
  TA_Integer outNbElement;

  for (int i = 0; i < size; ++i)
    closePrice[i] = i;

  TA_RetCode retCode = TA_MA(
      0, size - 1,
      &closePrice[0],
      3, TA_MAType_SMA,
      &outBeg, &outNbElement, &out[0]);
  /* The output is displayed here */
  for (int i = outBeg; i < outNbElement; ++i)
  {
    printf("Day %d = %f\n", outBeg + i, out[i]);
    EXPECT_NE(out[i], 0);
  }
}

TEST(talib, func_stoch)
{
  uint size = 100;
  TA_Real inHigh[size];
  TA_Real inLow[size];
  TA_Real inClose[size];
  TA_Real outK[size];
  TA_Real outD[size];
  TA_Integer outBeg;
  TA_Integer outNbElement;

  for (int i = 0; i < size; ++i)
  {
    inLow[i] = rand() % 100;
    inClose[i] = inLow[i] + rand() % 100;
    inHigh[i] = inClose[i] + rand() % 100;
  }

  TA_RetCode retCode = TA_STOCH(
      0, size - 1,
      &inHigh[0],
      &inLow[0],
      &inClose[0],
      25, 9,
      TA_MAType_SMA, 3, TA_MAType_SMA,
      &outBeg, &outNbElement,
      &outK[0], &outD[0]);
  /* The output is displayed here */
  for (int i = outBeg; i < outNbElement; ++i)
  {
    printf("(%f/%f/%f)Day %d = k: %f d: %f\n",
           inHigh[i], inLow[i], inClose[i],
           outBeg + i, outK[i], outD[i]);
    EXPECT_NE(outK[i], 0);
    EXPECT_NE(outD[i], 0);
    EXPECT_NE(outK[i], outD[i]);
  }
}

TEST(talib, func_stochf)
{
  uint size = 100;
  TA_Real inHigh[size];
  TA_Real inLow[size];
  TA_Real inClose[size];
  TA_Real outK[size];
  TA_Real outD[size];
  TA_Integer outBeg;
  TA_Integer outNbElement;

  for (int i = 0; i < size; ++i)
  {
    inLow[i] = rand() % 100;
    inClose[i] = inLow[i] + rand() % 100;
    inHigh[i] = inClose[i] + rand() % 100;
  }

  TA_RetCode retCode = TA_STOCHF(
      0, size - 1,
      &inHigh[0],
      &inLow[0],
      &inClose[0],
      25, 9,
      TA_MAType_SMA,
      &outBeg, &outNbElement,
      &outK[0], &outD[0]);
  /* The output is displayed here */
  for (int i = outBeg; i < outNbElement; ++i)
  {
    printf("(%f/%f/%f)Day %d = k: %f d: %f\n",
           inHigh[i], inLow[i], inClose[i],
           outBeg + i, outK[i], outD[i]);
    EXPECT_NE(outK[i], outD[i]);
  }
}

TEST(talib, func_table)
{
  TA_StringTable *table;
  TA_RetCode retCode;

  retCode = TA_GroupTableAlloc(&table);

  if (retCode == TA_SUCCESS)
  {
    for (int i = 0; i < table->size; ++i)
    {
      printf("%d/%d:\t %s\n", i + 1, table->size, table->string[i]);

      TA_StringTable *groupTable;
      TA_RetCode retCode2;

      retCode2 = TA_FuncTableAlloc(table->string[i], &groupTable);

      if (retCode2 == TA_SUCCESS)
      {
        for (int j = 0; j < groupTable->size; ++j)
        {
          printf("%d.%d/%d: %s\n", i + 1, j + 1, groupTable->size, groupTable->string[j]);
        }
      }
      else
      {
        printf("Grouptable allocation failed: %d\n", retCode2);
      }

      TA_FuncTableFree(groupTable);
    }

    TA_GroupTableFree(table);
  }
}

TEST(talib, func_abstract)
{
  TA_RetCode retCode;
  const TA_FuncHandle *funcHandle;
  const TA_FuncInfo *funcInfo;

  retCode = TA_GetFuncHandle("STOCH", &funcHandle);
  ASSERT_EQ(retCode, TA_SUCCESS);

  TA_ParamHolder *paramHolder;
  TA_ParamHolderAlloc(funcHandle, &paramHolder);
  ASSERT_EQ(retCode, TA_SUCCESS);

  retCode = TA_GetFuncInfo(funcHandle, &funcInfo);
  if (retCode == TA_SUCCESS)
  {
    printf("Name = %s\n", funcInfo->name);
    printf("CName = %s\n", funcInfo->camelCaseName);
    printf("Hint = %s\n", funcInfo->hint);
    printf("Nb Input = %d\n", funcInfo->nbInput);
    printf("Nb Optional Input = %d\n", funcInfo->nbOptInput);
    printf("Nb Output = %d\n", funcInfo->nbOutput);
  }

  const TA_InputParameterInfo *inputParamInfo;
  for (int i = 0; i < funcInfo->nbInput; i++)
  {
    printf("\n\nInput\n");
    TA_GetInputParameterInfo(funcInfo->handle, i, &inputParamInfo);
    printf("Param name: %s\n", inputParamInfo->paramName);
    printf("Param type: %#o\n", inputParamInfo->type);
    printf("Param flags: %#o\n", inputParamInfo->flags);
  }

  const TA_OutputParameterInfo *outputParamInfo;
  for (int i = 0; i < funcInfo->nbOutput; i++)
  {
    printf("\n\nOutput\n");
    TA_GetOutputParameterInfo(funcInfo->handle, i, &outputParamInfo);
    printf("Param name: %s\n", outputParamInfo->paramName);
    printf("Param type: %#o\n", outputParamInfo->type);
    printf("Param flags: %#o\n", outputParamInfo->flags);
  }

  uint size = 100;
  TA_Real inHigh[size];
  TA_Real inLow[size];
  TA_Real inClose[size];
  for (int i = 0; i < size; ++i)
  {
    inLow[i] = (rand() % 10000) / 100;
    inClose[i] = inLow[i] + (rand() % 10000) / 100;
    inHigh[i] = inClose[i] + (rand() % 10000) / 100;
  }

  retCode = TA_SetInputParamPricePtr(paramHolder, 0,
                                     nullptr,
                                     inHigh,
                                     inLow,
                                     inClose,
                                     nullptr,
                                     nullptr);
  ASSERT_EQ(retCode, TA_SUCCESS);

  TA_Real outK[size];
  TA_Real outD[size];
  TA_SetOutputParamRealPtr(paramHolder, 0, outK);
  TA_SetOutputParamRealPtr(paramHolder, 1, outD);

  // Call the function
  TA_Integer outBegIdx;
  TA_Integer outNbElement;
  retCode = TA_CallFunc(paramHolder, 0, size - 1,
                        &outBegIdx, &outNbElement);

  for (int i = outBegIdx; i < outNbElement; ++i)
  {
    printf("(%f/%f/%f)Day %d = k: %f d: %f\n",
           inHigh[i], inLow[i], inClose[i],
           outBegIdx + i, outK[i], outD[i]);
    EXPECT_NE(outK[i], outD[i]);
  }


  retCode = TA_ParamHolderFree(paramHolder);
  ASSERT_EQ(retCode, TA_SUCCESS);

  // if (retCode == TA_SUCCESS)
  // {
  //   retCode = TA_GetFuncInfo(funcHandle, &funcInfo);
  //   if (retCode == TA_SUCCESS)
  //     printf("Nb Input = %d\n", funcInfo->nbInput);
  // } else {
  //   printf("Could not get func info");
  // }

  // const char* xml = TA_FunctionDescriptionXML();
  // printf(xml);
}
