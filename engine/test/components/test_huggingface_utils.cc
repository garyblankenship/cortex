#include "gtest/gtest.h"
#include "utils/huggingface_utils.h"

class HuggingFaceUtilTestSuite : public ::testing::Test {};

TEST_F(HuggingFaceUtilTestSuite, TestGetModelRepositoryBranches) {
  auto branches =
      huggingface_utils::GetModelRepositoryBranches("cortexso", "tinyllama");

  EXPECT_EQ(branches.size(), 3);
  EXPECT_EQ(branches[0].name, "gguf");
  EXPECT_EQ(branches[0].ref, "refs/heads/gguf");
  EXPECT_EQ(branches[1].name, "1b-gguf");
  EXPECT_EQ(branches[1].ref, "refs/heads/1b-gguf");
  EXPECT_EQ(branches[2].name, "main");
  EXPECT_EQ(branches[2].ref, "refs/heads/main");
}

TEST_F(HuggingFaceUtilTestSuite, TestGetHuggingFaceModelRepoInfoSuccessfully) {
  auto model_info =
      huggingface_utils::GetHuggingFaceModelRepoInfo("cortexso", "tinyllama");
  auto not_null = model_info.has_value();

  EXPECT_TRUE(not_null);
  EXPECT_EQ(model_info->id, "cortexso/tinyllama");
  EXPECT_EQ(model_info->modelId, "cortexso/tinyllama");
  EXPECT_EQ(model_info->author, "cortexso");
  EXPECT_EQ(model_info->disabled, false);
  EXPECT_EQ(model_info->gated, false);

  auto tag_contains_gguf =
      std::find(model_info->tags.begin(), model_info->tags.end(), "gguf") !=
      model_info->tags.end();
  EXPECT_TRUE(tag_contains_gguf);

  auto contain_gguf_info = model_info->gguf.has_value();
  EXPECT_TRUE(contain_gguf_info);

  auto sibling_not_empty = !model_info->siblings.empty();
  EXPECT_TRUE(sibling_not_empty);
}

TEST_F(HuggingFaceUtilTestSuite,
       TestGetHuggingFaceModelRepoInfoReturnNullGgufInfoWhenNotAGgufModel) {
  auto model_info = huggingface_utils::GetHuggingFaceModelRepoInfo(
      "BAAI", "bge-reranker-v2-m3");
  auto not_null = model_info.has_value();

  EXPECT_TRUE(not_null);
  EXPECT_EQ(model_info->disabled, false);
  EXPECT_EQ(model_info->gated, false);

  auto tag_not_contain_gguf =
      std::find(model_info->tags.begin(), model_info->tags.end(), "gguf") ==
      model_info->tags.end();
  EXPECT_TRUE(tag_not_contain_gguf);

  auto contain_gguf_info = model_info->gguf.has_value();
  EXPECT_TRUE(!contain_gguf_info);

  auto sibling_not_empty = !model_info->siblings.empty();
  EXPECT_TRUE(sibling_not_empty);
}

TEST_F(HuggingFaceUtilTestSuite,
       TestGetHuggingFaceDownloadUrlWithoutBranchName) {
  auto downloadable_url = huggingface_utils::GetDownloadableUrl(
      "pervll", "bge-reranker-v2-gemma-Q4_K_M-GGUF",
      "bge-reranker-v2-gemma-q4_k_m.gguf");

  auto expected_url{
      "https://huggingface.co/pervll/bge-reranker-v2-gemma-Q4_K_M-GGUF/resolve/"
      "main/bge-reranker-v2-gemma-q4_k_m.gguf"};

  EXPECT_EQ(downloadable_url, expected_url);
}

TEST_F(HuggingFaceUtilTestSuite, TestGetHuggingFaceDownloadUrlWithBranchName) {
  auto downloadable_url = huggingface_utils::GetDownloadableUrl(
      "pervll", "bge-reranker-v2-gemma-Q4_K_M-GGUF",
      "bge-reranker-v2-gemma-q4_k_m.gguf", "1b-gguf");

  auto expected_url{
      "https://huggingface.co/pervll/bge-reranker-v2-gemma-Q4_K_M-GGUF/resolve/"
      "1b-gguf/bge-reranker-v2-gemma-q4_k_m.gguf"};

  EXPECT_EQ(downloadable_url, expected_url);
}