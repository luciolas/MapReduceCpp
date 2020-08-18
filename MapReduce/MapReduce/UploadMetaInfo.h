#pragma once

/// <summary>
/// To be used before stringing the chunks together. 
/// Contains information whether a batch of related chunks are complete
/// and 'string-able'.
/// </summary>
class UploadMetaInfo
{
public:
  size_t upload_id;
  size_t max_chunks;
  size_t highest_chunk_seen;
};